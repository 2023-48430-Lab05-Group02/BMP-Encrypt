import os
import sys
import re
import yaml

f = open(".exepath", "r")
build_dir = f.readline()

if sys.argv[1] == "true":
    interactive = True
else:
    interactive = False
print(f"Running in interactive mode: {interactive}")
if sys.argv[2] == "true":
    ignore_nonfatal = True
else:
    ignore_nonfatal = False

mode = sys.argv[3]

if mode == "read":
    errors = {"ERROR_QUESTIONABLE": [0, []],
              "ERROR_CORRUPT_AS_GOOD": [0, []]}

    print("---- TEST 1 - REAL EXAMPLES BMPs ----")
    valid_real_examples = os.listdir("./valid-examples")
    for file in valid_real_examples:
        print(f"-- TEST FILE: {file} --")
        out = os.popen(f"{build_dir} --encrypt --password test123 --input \"../test/valid-examples/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
        if out.find("Completed BMP encrypt") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
            print("-- PASSED TEST --")
        else:
            print(f"{out}\n-- Test Failed --\n")
            if interactive:
                input("-- PRESS ENTER FOR NEXT TEST --")
            else:
                try:
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)][0] += 1
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)][1].append("valid-examples/" + file)
                except KeyError:
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)] = [1, ["valid-examples/" + file]]

    print("---- TEST 2 - TEST CASE BMPs ----")
    valid_real_examples = os.listdir("./valid")
    for file in valid_real_examples:
        print(f"-- TEST FILE: {file} --")
        out = os.popen(f"{build_dir} --encrypt --password test123 --input \"../test/valid/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
        if out.find("Completed BMP encrypt") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
            print("-- PASSED TEST --")
        else:
            print(f"{out}\n-- Test Failed --\n")
            if interactive:
                input("-- PRESS ENTER FOR NEXT TEST --")
            else:
                try:
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)][0] += 1
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)][1].append("valid/" + file)
                except KeyError:
                    errors[re.search("An error has occurred reading the BMP Data:\n(.+)", out).group(1)] = [1, ["valid/" + file]]

    print("---- TEST 3 - QUESTIONABLE BMPs ----")
    valid_real_examples = os.listdir("./questionable")
    for file in valid_real_examples:
        print(f"-- TEST FILE: {file} --")
        out = os.popen(f"{build_dir} --encrypt --password test123 --input \"../test/questionable/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
        error_occurred = out.find("An error has occurred") != -1 and out.find("Successfully read bmp") == -1
        if error_occurred and not ignore_nonfatal:
            print("-- PASSED TEST --")
        elif error_occurred and ignore_nonfatal:
            print(f"{out}\n-- Test Failed --\n")
            if interactive:
                input("-- PRESS ENTER FOR NEXT TEST --")
            else:
                errors["ERROR_QUESTIONABLE"][0] += 1
                errors["ERROR_QUESTIONABLE"][1].append("questionable/" + file)
        elif not error_occurred and ignore_nonfatal:
            print("-- PASSED TEST --")
        elif not error_occurred and not ignore_nonfatal:
            print(f"{out}\n-- Test Failed --\n")
            if interactive:
                input("-- PRESS ENTER FOR NEXT TEST --")
            else:
                errors["ERROR_QUESTIONABLE"][0] += 1
                errors["ERROR_QUESTIONABLE"][1].append("questionable/" + file)

    print("---- TEST 4 - CORRUPT BMPs ----")
    valid_real_examples = os.listdir("./corrupt")
    for file in valid_real_examples:
        print(f"-- TEST FILE: {file} --")
        out = os.popen(f"{build_dir} --encrypt --password test123 --input \"../test/corrupt/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
        if out.find("An error has occurred") != -1 and out.find("Successfully read bmp") == -1:
            print("-- PASSED TEST --")
        else:
            print(f"{out}\n-- Test Failed --\n")
            if interactive:
                input("-- PRESS ENTER FOR NEXT TEST --")
            else:
                errors["ERROR_CORRUPT_AS_GOOD"][0] += 1
                errors["ERROR_CORRUPT_AS_GOOD"][1].append("corrupt/" + file)

    if not interactive:
        print("---- RESULTS ----")
        print(yaml.dump(errors))
