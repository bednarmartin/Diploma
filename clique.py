class CliqueFinder:

    def __init__(self, num_of_vertices, edges, mapping):
        self.store = [0] * num_of_vertices
        self.n = num_of_vertices
        self.graph = [[0 for _ in range(num_of_vertices)] for _ in range(num_of_vertices)]
        self.d = [0] * num_of_vertices
        self.mapping = mapping

        for i in range(len(edges)):
            self.graph[edges[i][0]][edges[i][1]] = 1
            self.graph[edges[i][1]][edges[i][0]] = 1
            self.d[edges[i][0]] += 1
            self.d[edges[i][1]] += 1

    def is_clique(self, b):

        for i in range(1, b):
            for j in range(i + 1, b):
                # If any edge is missing
                if self.graph[self.store[i]][self.store[j]] == 0:
                    return 0, []

        value = 0
        for index in self.store[1:b]:
            value += self.mapping[index]
        return value, self.store[1:b]

    def maxCliques(self, i, l):
        max_ = 0
        max_indices = []

        # Check if any vertices from i+1
        # can be inserted
        for j in range(i, self.n):

            self.store[l] = j

            # If the graph is not a clique of size k then
            # it cannot be a clique by adding another edge
            value, indices = self.is_clique(l + 1)
            if value > 0:
                if value > max_:
                    max_ = value
                    max_indices = indices

                # Check if another edge can be added
                value2, indices2 = self.maxCliques(j, l + 1)
                if value2 > max_:
                    max_ = value2
                    max_indices = indices2

        return max_, max_indices
