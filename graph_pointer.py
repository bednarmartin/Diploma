from files import list_files


class GraphPointer:
    representant = None
    edges = None
    filename: str = None
    number = None
    number_of_vertices = None

    def __init__(self, representant, edges, filename, number):
        self.representant = representant
        self.edges = edges
        if filename.count("/") == 0:
            self.filename = filename
        else:
            self.filename = filename.split("/")[-1]
        self.number = number
        self.number_of_vertices = int(self.filename[1:].split("_")[0])

    def __str__(self):
        return f"4 pole from file: {self.filename} of vertices {self.number_of_vertices}\nRepresentant of the 4 pole: {self.representant}\nDerived from the graph by cutting edges: {self.edges}\nNumber of graph: {self.number}"

    def __repr__(self):
        return self.__str__()


def find_graph(pointer: GraphPointer):
    all_files = list_files("inputs", ".txt")
    for file in all_files:
        if file.endswith(pointer.filename):
            with open(file, 'r') as input_file:
                start_index = 1 + ((pointer.number_of_vertices + 1) * (pointer.number - 1))
                end_index = start_index + pointer.number_of_vertices
                graph = input_file.readlines()[start_index:end_index]
                return graph
    print(f"Couldn't find graph {pointer.filename} number {pointer.number}")
    raise Exception()
