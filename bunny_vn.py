import numpy as np

input_filename = "bunny.obj"
output_filename = "bunny_light.obj"

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
for face in faces:
    a = vertices[face[0]-1]
    b = vertices[face[1]-1]
    c = vertices[face[2]-1]

    ab = b-a
    ac = c-a

    normal = np.cross(ab,ac)
    normal = normal/np.linalg.norm(normal)
    normals.append(normal)

f = open(output_filename, "w")
f.write("# {}\n".format(output_filename))
for v in vertices:
    f.write("v {} {} {}\n".format(v[0],v[1],v[2]))
for vn in normals:
    f.write("vn {} {} {}\n".format(vn[0],vn[1],vn[2]))
for face in faces:
    f.write("f {} {} {}\n".format(face[0],face[1],face[2]))