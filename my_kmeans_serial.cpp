//Author: Shailesh Tripathi

#include<bits/stdc++.h>

using namespace std;

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
		
		Data_tree(vector<Point> P)
		{
			points = P;
			left=NULL;
			right=NULL;
			int i,j;
			if(P.size() != 0)
			{
				
				for(i = 0 ; i<P[0].dimension ; i++)
				{
					min_C.push_back(DBL_MAX);
					max_C.push_back(DBL_MIN);
				}

				for(i = 0 ; i<P.size() ; i++)
				{
					for(j=0;j<P[i].dimension;j++)
					{
						min_C[j] = min(min_C[j], P[i].values[j]);
						max_C[j] = max(max_C[j], P[i].values[j]);
					}
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
		double center_sum;
		int count;
	
//	Centroid(vector<double> C)
//	{
//		values = C;
//		center_sum = 0;
//		count = 0;
//	}

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
	cout<<"min-max\n";	
	for(int i=0;i<total_attributes;i++)
	{
		cout<<node->min_C[i] << ' '<<node->max_C[i]<<endl;
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

//void prune(Data_tree *root,vector<Centroid>& C)
//{
	


int main()
{
	int  total_points,		//total number of points
		 total_attributes,  //dimension
		 K,					//number of clusters
		 max_iterations;	//maximum number of iterations

	int i,j;
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
	L[0] = 2;
	L[1] = 2;
	
	temp.values = L;
	C.push_back(temp);
		
	L[0] = 4;
	L[1] = 4;
	
	temp.values = L;
	C.push_back(temp);

	L[0] = 6;
	L[1] = 7;
	
	temp.values = L;
	C.push_back(temp);
	//done intialization

//	prune(root, C);	
	

}

