// K-means clustering algorithm. Implementaion using kd-tree
// Author: Shailesh Tripathi

#include<bits/stdc++.h>

using namespace std;

//utility function to compute euclidean distance between two vectors
double calc_dist(vector<double> A, vector<double> B)
{
	double res = 0;
	for(int i=0;i<A.size();i++)
		res += ((A[i]-B[i]) * (A[i]-B[i]));

	res = sqrt(res);
	return res;
}

//utility [A] = [A] + [B]
void add_vec(vector<double> &A, vector<double> &B)
{
	if(A.size() != B.size())
	{
		cout<<"Invalid addition of vectors\n";
		return ;
	} 
	for(int i=0;i<A.size();i++)
	{
		A[i]+=B[i];
	}
	return ;
}

class Point
{
	public:
		int id_point, id_cluster;
	    vector<double> values;
   		int dimension;
		
	Point(int id_point, vector<double>& values)
    {
        this->id_point = id_point;
        dimension = values.size();

        for(int i = 0; i < dimension; i++)
            this->values.push_back(values[i]);

        id_cluster = -1;
    }
	
};

class Data_tree
{
	public:
		Data_tree *left, *right;
		vector<Point> points;
		vector<double> min_C, max_C;
		vector<double> mid_C,wgt_cent;
		int num_points;
		

		Data_tree(vector<Point> P)
		{
			points = P;
			left=NULL;
			right=NULL;
			num_points = P.size();
			int i,j;
			if(P.size() != 0)
			{
			
				//initialize min, max nad mid for each dimension	
				for(i = 0 ; i<P[0].dimension ; i++)
				{
					min_C.push_back(DBL_MAX);
					max_C.push_back(DBL_MIN);
					mid_C.push_back(0);
				}

				for(i = 0 ; i<P.size() ; i++)
				{
					for(j=0;j<P[i].dimension;j++)
					{
						min_C[j] = min(min_C[j], P[i].values[j]);
						max_C[j] = max(max_C[j], P[i].values[j]);
						mid_C[j] += P[i].values[j];
					}
				}
				wgt_cent = mid_C;
				for(i = 0 ; i<P[0].dimension ; i++)	
				{
					
					mid_C[i] /= P.size();
				} 
			}
		}

		void display()
		{
			cout<<"size="<<points.size()<<endl;
		}

};

class Centroid
{
	public:
		vector<double> values;
		vector<double> center_sum;
		int count;
		int cent_id;
		int dimension;
	Centroid()
	{
//		values = C;
//		center_sum = 0;
		count = 0;
	}
	
	bool update()
	{	
		bool res =0;
		if(count != 0)
		{
			for(int i=0 ; i<center_sum.size();i++)
			{
				if( values[i] != center_sum[i]/count)
					res = 1;
				values[i] = center_sum[i]/count;
				center_sum[i] = 0;
			}
		count = 0;
		}
		return res;
	}
};

void print(vector<Point> P, int total_attributes)
{
	int i,j;
	for( i = 0 ; i < P.size() ; i++ )
	{
		for( j=0 ; j<total_attributes ; j++)
			cout<<P[i].values[j]<<' ';
		cout << endl;
	}
}

double find_median(vector<Point> P, int dim)
{
	vector<double> M;
	double median;

	for(int i=0; i< P.size() ;i++)
	{
		M.push_back(P[i].values[dim]);
	}
	sort(M.begin(), M.end());
	median = M[M.size()/2];

	return median;
}

void make_tree(Data_tree **node, vector<Point> P, int dim, int total_attributes)
{
	if(P.size() == 0)
		return;	

	//print(P,total_attributes);	
	//cout<<P.size()<<' '<<dim<<'\n';
	*node = new Data_tree(P);
	//(*node)->display();
	if( P.size() == 1)
		return;
	double median = find_median(P, dim);
	//cout<< "median = " << median<<endl;

	vector<Point> L,R;
	for(int i=0; i< P.size() ;i++)
	{
		if(P[i].values[dim] < median)
			L.push_back(P[i]);
		else
			R.push_back(P[i]);
	}
	

	make_tree(&((*node)->left), L, (dim+1)%total_attributes, total_attributes);
	make_tree(&((*node)->right), R, (dim+1)%total_attributes, total_attributes);
}

void iterate_tree(Data_tree *node, int total_attributes)
{
	if( node == NULL ) 
	{
		cout<<endl;
		return;
	}
	cout<<"min-max-mid\n";	
	for(int i=0;i<total_attributes;i++)
	{
		cout<<node->min_C[i] << ' '<<node->max_C[i]<<' '<<node->mid_C[i]<<endl;
	}

	cout<<"enter\n";
	for(int i=0; i< node->points.size() ;i++)
	{
		for(int j=0; j <total_attributes; j++)
			cout<< node->points[i].values[j]<<' ';
		cout<<endl;
	}
	iterate_tree(node->left, total_attributes);
	iterate_tree(node->right, total_attributes);
	
	cout<<"exit\n";
}

//returns true if z is farther than z* (z is a not potential center)
bool isFarther(Data_tree *root, Centroid z, Centroid z_star)
{
	int dim = root->points[0].dimension;
	int i;
	vector<double> corner_point(dim);
	
	//cout<<"corner ";
	for(i=0;i<dim;i++)
	{
		if(z.values[i] > z_star.values[i] ) 
			corner_point[i] = root->max_C[i];
		else
			corner_point[i] = root->min_C[i];
	
	//	cout<<corner_point[i] <<' '<<z_star.values[i]<<' '<< z.values[i]<<endl;
	}
	//cout<<endl;
	
	//cout<< calc_dist(corner_point,z.values) <<' '<< calc_dist(corner_point,z_star.values) << endl;	
	return calc_dist(corner_point,z.values) > calc_dist(corner_point,z_star.values);

}

void prune(Data_tree *node,vector<Centroid>& C, vector<int> ids)
{
	if (node == NULL)
		return;
	
	vector<int> pruned_id;
	int i,j;

	double dist,min_dist = DBL_MAX;
	int min_id;
	vector<double> corner(node->points.size());

//	cout<<"dist=";
	//find z*
	for(i =0; i < ids.size();i++)
	{
		dist =calc_dist(node->mid_C ,C[ids[i]].values);
//		cout<<dist<<' ';

	 	if(dist < min_dist)
		{
			min_dist = dist;
			min_id = ids[i];
		}
	}
//	cout<<endl;

	//if node is a leaf
	if(node->num_points == 1)
	{
		//add weights and centroid properties
		add_vec(C[min_id].center_sum, node->wgt_cent); 	//vector sum
		C[min_id].count += node->num_points;
		return;
	}

	pruned_id.push_back(min_id);
	
	for(i=0;i<ids.size();i++)
	{
		if(ids[i]!=min_id)
		{
			if(! isFarther(node, C[ids[i]], C[min_id] ))
			{
				pruned_id.push_back(ids[i]);
			}
		}
		
	}
	
//	for(i =0;i<pruned_id.size();i++)
//	{
//		cout<<pruned_id[i]<<' ';
//	}
//	cout<<endl;

	//if only one centroid left
	if(pruned_id.size() == 1)
	{
		//add weights and centroid properties
		add_vec(C[min_id].center_sum, node->wgt_cent);	//vector sum
		C[min_id].count += node->num_points;
	}
	else
	{
		prune(node->left , C, pruned_id);
		prune(node->right, C, pruned_id);
	} 

}	

//utility function to print centroids
void print_centroid_details(vector<Centroid> &C)
{
	for(int i=0; i <C.size();i++)
	{
		cout<<C[i].cent_id << ' '<< C[i].count<<endl;
		for(int j=0;j<C[i].center_sum.size();j++)
			cout<<C[i].values[j]<<' ';
		cout<<endl<<endl;
	}
}

bool update_centroid(vector<Centroid> &C)
{
	int i;
	bool res = 0;
	for(i=0;i<C.size();i++)
	{
		res |= C[i].update();
	}
	return res;
}

int main()
{
	int  total_points,		//total number of points
		 total_attributes,  //dimension
		 K,					//number of clusters
		 max_iterations;	//maximum number of iterations
	
	bool is_change;
	int i,j,iter;
	double value;
	Data_tree *root = NULL;

	cin >> total_points >> total_attributes >> K >> max_iterations;

	vector<Point> P;
	vector<double> values(total_attributes);

	//input the data points
	for(i = 0; i < total_points; i++)
	{

		for(j = 0; j < total_attributes; j++)
		{
			cin >> values[j];
		}

		Point p(i, values);
		P.push_back(p);
	}
	
	//build the data tree
	make_tree(&root, P, 0, total_attributes);

	//iterate over the tree and print it
	iterate_tree(root, total_attributes);

	
	//select random initial centroids
	vector<Centroid> C;
	
	//using test centers
	Centroid temp;
	vector<double> L(2);
	vector<double> zero_vec(2,0);
	L[0] = 2;
	L[1] = 2;
	
	temp.values = L;
	temp.cent_id = 0;
	temp.center_sum = zero_vec;
	C.push_back(temp);
		
	L[0] = 4;
	L[1] = 4;
	
	temp.values = L;
	temp.cent_id = 1;
	C.push_back(temp);

	L[0] = 6;
	L[1] = 7;
	
	temp.values = L;
	temp.cent_id = 2;
	C.push_back(temp);
	//done intialization

	vector<int> ids;
	ids.push_back(0);
	ids.push_back(1);
	ids.push_back(2);
	
	iter = 0;
	is_change = 1;
	
	while( (iter++ < max_iterations) && is_change)
	{
		prune(root, C,ids);	
		is_change = update_centroid(C);
		cout<<is_change<<endl;
		cout<<"after\n";
		print_centroid_details(C);
	}
}
