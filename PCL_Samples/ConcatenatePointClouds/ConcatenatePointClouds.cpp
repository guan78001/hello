#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
int
main (int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "please specify command line arg '-f' or '-p'" << std::endl;
    exit(0);
  }
  pcl::PointCloud<pcl::PointNormal> cloud_a, cloud_b, cloud_c;
  pcl::PointCloud<pcl::Normal> n_cloud_b;
  pcl::PointCloud<pcl::PointNormal> p_n_cloud_c;

  int n = 3;
  cloud_a.points.resize(n);
  cloud_b.points.resize(n);
  for (int i = 0; i < cloud_a.points.size(); i++) {
    float r = 0.06f;
    cloud_a.points[i].x = i * r;
    cloud_a.points[i].y = i * r;
    cloud_a.points[i].z = i * r;
    cloud_a.points[i].normal_x = 0;
    cloud_a.points[i].normal_y = 1;
    cloud_a.points[i].normal_z = 0;
  }
  for (int i = 0; i < cloud_b.points.size(); i++) {
    float r = 0.05f;
    cloud_b.points[i].x = i * r;
    cloud_b.points[i].y = i * r;
    cloud_b.points[i].z = i * r;

    cloud_b.points[i].normal_x = 0;
    cloud_b.points[i].normal_y = 0;
    cloud_b.points[i].normal_z = 1;
  }
  cloud_c = cloud_a + cloud_b;
  //filter
  typedef pcl::PointNormal PointT;
  typedef pcl::PointCloud<pcl::PointNormal> CloudT;

  CloudT::Ptr cloud(new CloudT());
  CloudT::Ptr cloud_filtered(new CloudT());

  // Create the filtering object
  pcl::VoxelGrid<PointT> sor;
  *cloud = cloud_c;
  sor.setInputCloud(cloud);
  const float grid_size = 0.05f;
  sor.setLeafSize(grid_size, grid_size, grid_size);
  sor.filter(*cloud_filtered); // <==== culprit

  std::cout << "Filter:" << std::endl;
  for (size_t i = 0; i < cloud_filtered->points.size(); ++i)
    std::cerr << "    (" <<
              cloud_filtered->points[i].x << " " << cloud_filtered->points[i].y << " " << cloud_filtered->points[i].z << "), (" <<
              cloud_filtered->points[i].normal_x << " " << cloud_filtered->points[i].normal_y << " " << cloud_filtered->points[i].normal_z
              << ") " << std::endl;
  return (0);
}