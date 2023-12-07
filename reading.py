from files import list_files
from graph_pointer import GraphPointer


class GraphReader:

    def __init__(self, representant, files):
        self.representant = representant
        self.files = files

    def get_graph_pointers(self):
        all_files = list_files("outputs/snarks", ".txt")
        graph_pointers = []
        for file in all_files:
            if file.split("/")[-1].strip() in self.files:
                with open(file, 'r') as file_to_read:
                    actual_graph_number = None
                    actual_file_name = None
                    for line in file_to_read.readlines():
                        split_line = line.split(" - ")
                        if split_line[0].strip().startswith("GRAPH"):
                            actual_graph_number = int(split_line[0].strip().split(" ")[1].strip())
                            if '/' in split_line[1].strip().split():
                                actual_file_name = split_line[1].strip().split("/")[-1].strip()
                            else:
                                actual_file_name = split_line[1].strip()
                        elif split_line[0].strip() == str(self.representant):
                            tmp = split_line[1].strip().split(",")
                            first_edge = [int(s) for s in tmp[0][1:-1].split() if s.isdigit()]
                            second_edge = [int(s) for s in tmp[1][1:-1].split() if s.isdigit()]
                            graph_pointer = GraphPointer(representant=self.representant,
                                                         edges=[first_edge, second_edge],
                                                         filename=actual_file_name,
                                                         number=actual_graph_number)
                            graph_pointers.append(graph_pointer)
        return graph_pointers
