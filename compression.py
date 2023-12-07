from files import convert_grf_to_txt, list_files
from mapping import Mapping
import subprocess
from clique import CliqueFinder


def compress_all_graphs_with_number_of_vertices(num_of_vertices):
    dangling_edges_vertices = {
        1: [0, 7, 30, 32],
        2: [0, 7, 9, 13, 16],
        3: [0, 7, 9, 8, 10],
        4: [0, 7, 2, 5],
        5: [1, 6, 4, 3],
        6: [4, 5, 7, 8, 8, 9],
        7: [0, 7, 19, 21, 22],
        8: [22, 31, 0, 25]
    }
    path = f"/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/all/{num_of_vertices}/unique"
    file_locations = list_files(path, ".grf")
    for file_location in file_locations:
        all_mappings = set()
        for t in dangling_edges_vertices.keys():
            dangling_edges = dangling_edges_vertices[t]
            cmd = "vf3lib/bin/vf3"
            grf_file1 = f"vf3lib/inputs/type_{t}.grf"
            result = subprocess.run(
                [cmd, grf_file1, file_location, "-u", "-s"], text=True, capture_output=True)
            lines = result.stdout.splitlines()
            if len(lines) != 2:
                for line in lines[1:-1]:
                    vertices = set()
                    dangling_edges_mapping = [-1] * len(dangling_edges)
                    for mapping in line.strip().split(":")[:-1]:
                        sub_vertex = int(mapping.split(',')[1])
                        vertex = int(mapping.split(',')[0])
                        if sub_vertex in dangling_edges:
                            starting_position = 0
                            condition = True
                            while condition:
                                try:
                                    index = dangling_edges[starting_position:].index(sub_vertex)
                                    if dangling_edges_mapping[starting_position + index] == -1:
                                        dangling_edges_mapping[starting_position + index] = vertex
                                    else:
                                        starting_position += index + 1
                                except:
                                    break
                        vertices.add(vertex)
                    all_mappings.add(Mapping(frozenset(vertices), tuple(dangling_edges_mapping), t))
        all_mappings_list = list(all_mappings)
        disjoint_mappings_list = []
        for i in range(len(all_mappings_list)):
            for j in range(i, len(all_mappings_list)):
                if all_mappings_list[i].vertices.isdisjoint(all_mappings_list[j].vertices):
                    disjoint_mappings_list.append([all_mappings_list[i], all_mappings_list[j]])
        edges = []
        for disjoint in disjoint_mappings_list:
            edges.append([all_mappings_list.index(disjoint[0]), all_mappings_list.index(disjoint[1])])
        n = len(all_mappings_list)
        mapping = dict()
        M = {
            1: 34,
            2: 19,
            3: 11,
            4: 8,
            5: 10,
            6: 10,
            7: 23,
            8: 32
        }
        for i in range(len(all_mappings_list)):
            mapping[i] = M[all_mappings_list[i].type]
        finder = CliqueFinder(n, edges, mapping)
        num, map_indices = finder.maxCliques(0, 1)
        print(file_location.strip().split("/")[-1])
        print()
        mappings = []
        for index in map_indices:
            print(all_mappings_list[index])
            mappings.append(all_mappings_list[index])
        print()
        compress_graph(file_location, mappings, dangling_edges_vertices)
        print()
        print()


def compress_graph(graph_location, mappings, dangling_edges_vertices):
    compress_mapping = []
    convert_grf_to_txt(graph_location, "tmp.txt")
    with open("tmp.txt", 'r') as file:
        lines = file.readlines()
        edges_between_compressed_and_regular_vertices = []
        remaining_vertices = set(i for i in range(int(lines[0].strip())))
        dangling_edges_connections = []
        for i in range(len(mappings)):
            for vertex in mappings[i].vertices:
                remaining_vertices.remove(vertex)

        for i in range(len(mappings)):
            connected_vertices = mappings[i].dangling_edges_mapping

            for connected_vertex in connected_vertices:
                adjacent_vertices = [int(v) for v in lines[connected_vertex + 1].strip().split(" ")]
                for adjacent_vertex in adjacent_vertices:
                    if adjacent_vertex in remaining_vertices:
                        edges_between_compressed_and_regular_vertices.append((i, adjacent_vertex))
                        dangling_edges_connections.append((connected_vertex, adjacent_vertex))

            for j in range(i + 1, len(mappings)):
                vertices = mappings[j].vertices
                for connected_vertex in connected_vertices:
                    adjacent_vertices = [int(v) for v in lines[connected_vertex + 1].strip().split(" ")]
                    for adjacent_vertex in adjacent_vertices:
                        if adjacent_vertex in vertices:
                            compress_mapping.append((i, j))
                            dangling_edges_connections.append((connected_vertex, adjacent_vertex))

        counts_between_compressed_vertices = {}
        for element in compress_mapping:
            if element not in counts_between_compressed_vertices.keys():
                counts_between_compressed_vertices[element] = compress_mapping.count(element)
        regular_edges = []

        counts_between_compressed_and_regular_vertices = {}
        for element in edges_between_compressed_and_regular_vertices:
            if element not in counts_between_compressed_and_regular_vertices.keys():
                counts_between_compressed_and_regular_vertices[
                    element] = edges_between_compressed_and_regular_vertices.count(element)

        for remaining_vertex in remaining_vertices:
            adjacent_vertices = [int(v) for v in lines[remaining_vertex + 1].strip().split(" ")]
            for adjacent_vertex in adjacent_vertices:
                if adjacent_vertex in remaining_vertices:
                    regular_edges.append([remaining_vertex, adjacent_vertex])

        map_compressed_to_vertices = {}
        type_counters = {}
        dangling_vertices_mapping = {}

        for i in range(len(mappings)):
            dangling_edges_mapping = mappings[i].dangling_edges_mapping
            type = mappings[i].type
            if type in type_counters.keys():
                map_compressed_to_vertices[i] = f"{type}.{type_counters[type]}"
                for j in range(len(dangling_edges_vertices[type])):
                    dangling_vertices_mapping[
                        dangling_edges_mapping[j]] = f"{type}.{type_counters[type]}.{dangling_edges_vertices[type][j]}"
                type_counters[type] += 1
            else:
                map_compressed_to_vertices[i] = f"{type}.0"
                for j in range(len(dangling_edges_mapping)):
                    dangling_vertices_mapping[
                        dangling_edges_mapping[j]] = f"{type}.0.{dangling_edges_vertices[type][j]}"
                type_counters[type] = 1
        """
        print("===VIZUAL===")
        for key in counts_between_compressed_vertices.keys():
            print(
                f"{map_compressed_to_vertices[key[0]]} {map_compressed_to_vertices[key[1]]} {counts_between_compressed_vertices[key]}")
        for key in counts_between_compressed_and_regular_vertices.keys():
            print(
                f"{map_compressed_to_vertices[key[0]]} {key[1]} {counts_between_compressed_and_regular_vertices[key] if counts_between_compressed_and_regular_vertices[key] != 1 else ''}")
        for edge in regular_edges:
            print(f"{edge[0]} {edge[1]}")
        """
        for dangling_edge_connection in dangling_edges_connections:
            if dangling_edge_connection[0] in dangling_vertices_mapping.keys():
                if dangling_edge_connection[1] in dangling_vertices_mapping.keys():
                    print(
                        f"{dangling_vertices_mapping[dangling_edge_connection[0]]} {dangling_vertices_mapping[dangling_edge_connection[1]]}")
                else:
                    print(
                        f"{dangling_vertices_mapping[dangling_edge_connection[0]]} {dangling_edge_connection[1]}")
            else:
                if dangling_edge_connection[1] in dangling_vertices_mapping.keys():
                    print(f"{dangling_edge_connection[0]} {dangling_vertices_mapping[dangling_edge_connection[1]]}")
                else:
                    print(f"{dangling_edge_connection[0]} {dangling_edge_connection[1]}")
        for edge in regular_edges:
            print(f"{edge[0]} {edge[1]}")
