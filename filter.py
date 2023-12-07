from files import list_files


def get_number():
    print(sum(1 for i in open("KempeOutput.txt", 'rb')))


def filter_file(file_path):
    with open(file_path, 'r') as file:
        numbers = set()
        for line in file:
            x = line.split(" ")[0]
            if x != '\n' and x != 'GRAPH':
                numbers.add(int(x))
        file_name = file_path.split("/")[-1]
        with open(f"filtered/{file_name}", 'w') as outputFile:
            for number in sorted(numbers):
                outputFile.write(str(number))
                outputFile.write("\n")


def validate():
    with open(f"ALLsnarksFILTEREDSET.txt", 'r') as file:
        with open("KempeOutput.txt", 'r') as kempe_file:
            lines = file.readlines()
            index = 0
            for kempe_line in kempe_file:
                kempe_number = int(kempe_line.split("-")[0])
                actual_number = int(lines[index])
                if kempe_number < actual_number:
                    continue
                elif kempe_number == actual_number:
                    print(actual_number, " is in Kempe File")
                    index += 1
                    if index >= len(lines):
                        return
                else:
                    print(kempe_number)
                    raise Exception(actual_number)



