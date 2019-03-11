struct job_node
{
	//this in essence makes a graph,
	//but of manafucturing and not of jobs!
	int number_needed;
	int resources_needed;
	int resulting_resource;
};
struct job
{
	Job_node* job_nodes;//bring rocks to factory, make rock to chair.
}
