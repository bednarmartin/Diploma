import subprocess
import sys

from compression import compress_all_graphs_with_number_of_vertices

arguments = sys.argv
if arguments[1] == 'get_representant':
    input_file_path = arguments[2]
    output_file_path = arguments[3]
    cmd = "algorithm/output.out"
    result = subprocess.run(
        [cmd, "test", input_file_path, output_file_path], text=True, stdout=sys.stdout)

elif arguments[1] == 'get_compressed_graph':
    number_of_vertices = int(arguments[2])
    compress_all_graphs_with_number_of_vertices(number_of_vertices)
