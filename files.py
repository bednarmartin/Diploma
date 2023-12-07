import os


def list_files(directory, extension):
    txt_files = []

    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(extension):
                txt_files.append(os.path.join(root, file))

    return txt_files


def convert_snarks_to_txt(file_path, number_of_vertices):
    with open(file_path, 'r') as file:
        with open(f"inputs/s{number_of_vertices}_c4.txt", 'w') as write_file:
            write_file.write(f"{number_of_vertices}\n")
            index = 0
            first_line = None
            second_line = None
            third_line = None
            for line in file.readlines():
                index += 1
                if index == 1:
                    continue
                elif index == 2:
                    first_line = [(int(s) - 1) for s in line.split()]
                    continue
                elif index == 3:
                    second_line = [(int(s) - 1) for s in line.split()]
                    continue
                elif index == 4:
                    third_line = [(int(s) - 1) for s in line.split()]
                    index = 0
                for i in range(number_of_vertices):
                    write_file.write(f"{first_line[i]} {second_line[i]} {third_line[i]}\n")
                write_file.write("\n")


def convert_txt_to_grf(input_path, output_path):
    with open(input_path, 'r') as read_file:
        with open(output_path, 'w') as write_file:
            counter = -1
            for line in read_file.readlines():
                if counter == -1:
                    number_of_vertices = int(line.strip())
                    write_file.write(f"{number_of_vertices}\n")
                    for i in range(number_of_vertices):
                        write_file.write(f"{i} 1\n")
                else:
                    vertices = [int(i) for i in line.strip().split(" ") if int(i) > counter]
                    write_file.write(f"{len(vertices)}\n")
                    for i in range(len(vertices)):
                        write_file.write(f"{counter} {vertices[i]}\n")
                counter += 1


def convert_grf_to_txt(input_path, output_path):
    with open(input_path, 'r') as input_file:
        with open(output_path, 'w') as output_file:
            lines = input_file.readlines()
            num_of_vertices = int(lines[0].strip())
            hash_map = {}
            for line in lines[num_of_vertices + 2:]:
                tmp = line.strip().split(" ")
                if len(tmp) == 2:
                    vertex1 = int(tmp[0].strip())
                    vertex2 = int(tmp[1].strip())
                    if vertex1 in hash_map.keys():
                        hash_map[vertex1].add(vertex2)
                    else:
                        hash_map[vertex1] = {vertex2}

                    if vertex2 in hash_map.keys():
                        hash_map[vertex2].add(vertex1)
                    else:
                        hash_map[vertex2] = {vertex1}
            output_file.write(f"{num_of_vertices}\n")
            for i in range(num_of_vertices):
                assert len(hash_map[i]) == 3
                vertices = sorted(list(hash_map[i]))
                output_file.write(f"{vertices[0]} {vertices[1]} {vertices[2]}\n")


def convert_to_visualize(file_path, output_file_path):
    with open(file_path, 'r') as file:
        with open(output_file_path, 'w') as output_file:
            for index, line in enumerate(file.readlines()[1:]):
                numbers = [int(i) for i in line.strip().split(" ") if int(i) > index]
                for number in numbers:
                    output_file.write(f"{index} {number}\n")
