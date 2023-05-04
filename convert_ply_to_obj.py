import open3d as o3d

def ply_to_obj(ply_file, obj_file):
    mesh = o3d.io.read_triangle_mesh(ply_file)

    o3d.io.write_triangle_mesh(obj_file, mesh)

input_ply_file = "./bunny/reconstruction/bun_zipper_res4.ply"
output_obj_file = "bunny.obj"

ply_to_obj(input_ply_file,output_obj_file)

