class Mapping:

    def __init__(self, vertices: frozenset, dangling_edges_mapping, type: int):
        self.vertices = vertices
        self.dangling_edges_mapping = dangling_edges_mapping
        self.type = type

    def __str__(self):
        return f"{self.vertices} {self.dangling_edges_mapping}; type={self.type}"

    def __repr__(self):
        return f"{self.vertices} {self.dangling_edges_mapping}; type={self.type}"

    def __hash__(self):
        return hash(frozenset({self.vertices, self.type}))

    def __eq__(self, other):
        return self.vertices == other.vertices and self.type == other.type