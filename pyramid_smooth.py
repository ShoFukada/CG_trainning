import numpy as np

input_filename = "pyramid.obj"
output_filename = "pyramid_smooth.obj"

vertices = []
normals = []
faces = []
file_data = open(input_filename,"r")
for line in file_data:
    if line.startswith('v '):
        vertices.append(list(map(float, line.split()[1:])))
    elif line.startswith('f '):
        faces.append(list(map(int, line.split()[1:])))
vertices = np.array(vertices)
faces = np.array(faces)
vertex_normals = [np.array([0.0,0.0,0.0]) for _ in range(len(vertices))]
print(vertex_normals)
for face in faces:
    a = vertices[face[0]-1]
    b = vertices[face[1]-1]
    c = vertices[face[2]-1]

    ab = b-a
    ac = c-a

    normal = np.cross(ab,ac)
    normal = normal/np.linalg.norm(normal)
    
    vertex_normals[face[0]-1] += normal
    vertex_normals[face[1]-1] += normal
    vertex_normals[face[2]-1] += normal
    print(vertex_normals)
print()
vertex_normals = [vn / np.linalg.norm(vn) for vn in vertex_normals]
print(vertex_normals)
print(vertex_normals[2][2])
f = open(output_filename, "w")
f.write("# {}\n".format(output_filename))
for v in vertices:
    f.write("v {} {} {}\n".format(v[0],v[1],v[2]))
for vn in vertex_normals:
    f.write("vn {} {} {}\n".format(vn[0],vn[1],vn[2]))
for face in faces:
    f.write("f {} {} {}\n".format(face[0],face[1],face[2]))