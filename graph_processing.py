import itertools
from typing import List, TextIO

from files import list_files, convert_grf_to_txt
from graph_pointer import find_graph
from reading import GraphReader
import os, subprocess, shutil


def process_file(filename):
    with open(filename, 'r') as readfile:
        with open(f"processed_{filename}", 'w') as writefile:
            counter = 0
            for line in readfile.readlines():
                counter += 1
                if counter <= 3:
                    continue
                elif counter <= 35:
                    writefile.write(line)
                else:
                    writefile.write("\n")
                    counter = 1
def load_disjunct_representants():
    pairs = []
    with open("inputs/disjunctRepresentants.txt", 'r') as file:
        for line in file.readlines():
            pair = line.split(",")
            pair[0] = int(pair[0])
            pair[1] = int(pair[1])
            pairs.append(pair)
    return pairs


def get_hash_map():
    hashmap = {}
    filtered_file_paths = list_files("filtered", ".txt")
    for filtered_file_path in filtered_file_paths:
        with open(filtered_file_path, 'r') as file:
            lines = file.readlines()
            for line in lines:
                number = int(line.split("\n")[0])
                if number in hashmap:
                    hashmap[number].add(filtered_file_path.split("/")[-1])
                else:
                    hashmap[number] = set()
                    hashmap[number].add(filtered_file_path.split("/")[-1])
    return hashmap


def process_all_graphs():
    hashmap = get_hash_map()
    pairs = load_disjunct_representants()
    counter = 1
    pair_counter = 0
    for pair in pairs:
        pair_counter += 1
        graph_reader = GraphReader(pair[0], hashmap[pair[0]])
        graph_reader2 = GraphReader(pair[1], hashmap[pair[1]])
        graph_pointers = graph_reader.get_graph_pointers()
        graph_pointers2 = graph_reader2.get_graph_pointers()

        for i in range(len(graph_pointers)):
            graph1 = find_graph(graph_pointers[i])
            for j in range(len(graph_pointers2)):
                graph2 = find_graph(graph_pointers2[j])

                path = os.path.join("extracted/individual", str(counter))
                try:
                    os.mkdir(path)
                except:
                    pass

                with open(f"{path}/graph1.txt", 'w') as file:
                    file.write(f"{graph_pointers[i].number_of_vertices}\n")
                    for line in graph1:
                        file.write(line)

                with open(f"{path}/graph2.txt", 'w') as file:
                    file.write(f"{graph_pointers2[j].number_of_vertices}\n")
                    for line in graph2:
                        file.write(line)

                with open(f"{path}/graph1_info.txt", 'w') as file:
                    file.write(f"{graph_pointers[i]}")

                with open(f"{path}/graph2_info.txt", 'w') as file:
                    file.write(f"{graph_pointers2[j]}")

                counter += 1
                print(counter)


def get_unique(number_of_vertices):
    grf_files = list_files(f"extracted/all/{number_of_vertices}/", ".grf")
    buckets = [0] * len(grf_files)
    path = f"/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/all/{number_of_vertices}"
    path2 = os.path.join(path, "unique")

    try:
        os.mkdir(path2)
    except:
        pass

    path3 = os.path.join(path2, "txts")

    try:
        os.mkdir(path3)
    except:
        pass

    for i in range(len(grf_files)):
        print(f"Processing {i}/{len(grf_files)}")
        if buckets[i] == 1:
            continue
        grf_file1 = grf_files[i]
        for j in range(i + 1, len(grf_files)):
            if buckets[j] == 1:
                continue
            grf_file2 = grf_files[j]
            cmd = "/home/bednarmartin/diplomaThesis/CLionProjects/PerfectMatchingIndex/output.out"
            result = subprocess.run(
                [cmd, "test_isomorphism", grf_file1, grf_file2], text=True, capture_output=True)
            if result.stdout == "1\n":
                buckets[j] = 1

    for i in range(len(grf_files)):
        if buckets[i] == 0:
            shutil.copy(grf_files[i], f"{path2}/{grf_files[i].strip().split('/')[-1].strip()}")
            convert_grf_to_txt(grf_files[i], f"{path3}/{grf_files[i].strip().split('/')[-1].strip()[:-4]}.txt")


def extract_vertices(graph_info_file):
    graph_edges_tmp = graph_info_file.readlines()[-2].split(": ")[-1].strip()[2:-2].split(", ")
    vertex1 = int(graph_edges_tmp[0])
    vertex2 = int(graph_edges_tmp[1].split("]")[0])
    vertex3 = int(graph_edges_tmp[2].split("[")[1])
    vertex4 = int(graph_edges_tmp[3].split("]")[0])
    return [vertex1, vertex2, vertex3, vertex4]


def prepare(graph_file1: TextIO, vertices1: List[int], graph_file2: TextIO, vertices2: List[int]):
    graph1_lines = []
    for line in graph_file1.readlines():
        input_line = [int(i.strip()) for i in line.split(" ")]
        if len(input_line) == 1:
            continue
        graph1_lines.append(input_line)

    graph1_lines[vertices1[0]].remove(vertices1[1])
    graph1_lines[vertices1[1]].remove(vertices1[0])
    graph1_lines[vertices1[2]].remove(vertices1[3])
    graph1_lines[vertices1[3]].remove(vertices1[2])

    graph2_lines = []
    for line in graph_file2.readlines():
        input_line = [int(i.strip()) for i in line.split(" ")]
        if len(input_line) == 1:
            continue
        graph2_lines.append(input_line)

    graph2_lines[vertices2[0]].remove(vertices2[1])
    graph2_lines[vertices2[1]].remove(vertices2[0])
    graph2_lines[vertices2[2]].remove(vertices2[3])
    graph2_lines[vertices2[3]].remove(vertices2[2])

    return graph1_lines, graph2_lines


def remove_and_add_edges(graph1_lines, vertices1: List[int], graph2_lines, vertices2: List[int],
                         permutation: List[int]):
    graph1_length = len(graph1_lines)
    graph2_length = len(graph2_lines)

    for line in graph2_lines:
        for i in range(len(line)):
            line[i] += graph1_length

    graph2_lines[vertices2[0]].append(vertices1[permutation.index(1)])
    graph2_lines[vertices2[1]].append(vertices1[permutation.index(2)])
    graph2_lines[vertices2[2]].append(vertices1[permutation.index(3)])
    graph2_lines[vertices2[3]].append(vertices1[permutation.index(4)])
    graph1_lines[vertices1[permutation.index(1)]].append(vertices2[0] + graph1_length)
    graph1_lines[vertices1[permutation.index(2)]].append(vertices2[1] + graph1_length)
    graph1_lines[vertices1[permutation.index(3)]].append(vertices2[2] + graph1_length)
    graph1_lines[vertices1[permutation.index(4)]].append(vertices2[3] + graph1_length)

    new_graph = [f"{graph1_length + graph2_length}\n"]

    for line in graph1_lines:
        line.sort()
        new_graph.append(f"{line[0]} {line[1]} {line[2]}\n")

    for line in graph2_lines:
        line.sort()
        new_graph.append(f"{line[0]} {line[1]} {line[2]}\n")

    return graph1_length + graph2_length, new_graph

def make_graphs():
    for i in range(57141, 60000):
        print(f"Working on i = {i}")
        path = f"/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/individual/{i}"
        path2 = os.path.join(path, "graphs")
        try:
            os.mkdir(path2)
        except:
            pass
        with open(f"{path}/graph1_info.txt", 'r') as graph1_info_file:
            vertices1 = extract_vertices(graph1_info_file)
            with open(f"{path}/graph2_info.txt", 'r') as graph2_info_file:
                counter = 0
                vertices2 = extract_vertices(graph2_info_file)
                with open(f"{path}/graph1.txt", 'r') as graph1_file:
                    with open(f"{path}/graph2.txt", 'r') as graph2_file:
                        graph1_lines, graph2_lines = prepare(graph1_file, vertices1, graph2_file, vertices2)
                        for permutation in list(itertools.permutations([1, 2, 3, 4])):
                            num_of_vertices, new_graph = remove_and_add_edges(shutil.copy.deepcopy(graph1_lines),
                                                                              vertices1,
                                                                              shutil.copy.deepcopy(graph2_lines),
                                                                              vertices2,
                                                                              list(permutation))
                            with open(f"{path}/graphs/new_graph{counter}.txt", 'w') as write_file:
                                for line in new_graph:
                                    write_file.write(line)

                            first_argument = "test_is_unsatisfiable"
                            second_argument = f"{path}/graphs/new_graph{counter}.txt"

                            cmd = "/home/bednarmartin/diplomaThesis/CLionProjects/PerfectMatchingIndex/output.out"
                            result = subprocess.run(
                                [cmd, first_argument, second_argument], text=True, capture_output=True)

                            if result.stdout == "UNSATISFIABLE\n":
                                counter += 1
                            else:
                                os.remove(second_argument)
                        if counter == 0:
                            raise Exception(f"{i} went wrong")
