/*struct Factory
{
	Factory_type type;
	vec2i pos;
	
}
struct hash_table//we'll start with hash_table of pointers.
{
	int capacity;
	int count;
	void* values;
}*/
enum Product_type
{
	Product_type_rock,
	Product_type_chair,//TODO define properties in order to make the cases clearer.
	NUM_OF_PRODUCT_TYPES
};
struct Job_product
{
	Product_type product;
	int number;
};
struct Job_node
{
	//this in essence makes a graph,
	//but of manafucturing and not of jobs!
	Job_product* resources_needed;
	int count_resources_needed;
	Product_type resulting_resource;
	int dwarfs_working_on_it;
	int max_dwarfs_working_on_it;
};
struct New_job
{
	Job_node* job_nodes;//bring rocks to factory, make rock to chair.
	int count;
};

