from files import list_files, convert_txt_to_grf
import subprocess, os, shutil


def get_unique_permutation_graphs():
    for i in range(57141, 250000):
        print(f"processing i = {i}")
        path = f"/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/individual/{i}/graphs"
        path2 = os.path.join(path, "grfs")
        counter = 0
        try:
            os.mkdir(path2)
        except:
            pass
        for j in range(24):
            try:
                input_path = f"{path}/new_graph{j}.txt"
                output_path = f"{path2}/new_graph{j}.grf"
                convert_txt_to_grf(input_path, output_path)
            except:
                counter = j
                break

        hash_map = {}
        for j in range(counter):
            hash_map[j] = 0
        for j in range(counter):
            if hash_map[j] == 1:
                continue
            file1_path = f"{path2}/new_graph{j}.grf"
            for k in range(j + 1, counter):
                if hash_map[k] == 1:
                    continue
                file2_path = f"{path2}/new_graph{k}.grf"
                cmd = "/home/bednarmartin/diplomaThesis/CLionProjects/PerfectMatchingIndex/output.out"
                result = subprocess.run(
                    [cmd, "test_isomorphism", file1_path, file2_path], text=True, capture_output=True)
                if result.stdout == "1\n":
                    hash_map[k] = 1
        second_counter = 1
        for j in range(counter):
            with open(f"{path}/new_graph{j}.txt", 'r') as file:
                num_of_vertices = int(file.readlines()[0].strip())
                if hash_map[j] == 0:
                    shutil.copy(f"{path2}/new_graph{j}.grf",
                                f"/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/all/{num_of_vertices}/{i}_{second_counter}.grf")
                    second_counter += 1


def test_with_existing():
    path = "/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/all/52/unique/txts/test"
    path2 = "/home/bednarmartin/DiplomaThesisStuff/DiplomaThesis/RepresentantChecker/extracted/all/52/unique/txts/test/grfs"
    txt_files = list_files(path, ".txt")
    for txt_file in txt_files:
        convert_txt_to_grf(txt_file, f"{path2}/{txt_file.split('/')[-1].strip()[:-4]}.grf")
    grf_files = sorted(list_files(path2), ".grf")
    buckets = [0] * len(grf_files)
    for i in range(len(grf_files)):
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
            print(grf_files[i])
