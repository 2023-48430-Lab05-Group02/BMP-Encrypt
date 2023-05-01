import os
import sys
import re
import yaml

if sys.argv[1] == "true":
    interactive = True
else:
    interactive = False
print(f"Running in interactive mode: {interactive}")
if sys.argv[2] == "true":
    ignore_nonfatal = True
else:
    ignore_nonfatal = False

errors = {"ERROR_QUESTIONABLE_AS_GOOD": [0, []],
          "ERROR_CORRUPT_AS_GOOD": [0, []]}

print("---- TEST 1 - REAL EXAMPLES BMPs ----")
valid_real_examples = os.listdir("./valid-examples")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/valid-examples/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
    if out.find("Successfully read bmp") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
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
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/valid/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
    if out.find("Successfully read bmp") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
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
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/questionable/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
    error_occurred = out.find("An error has occurred") != -1 and out.find("Successfully read bmp") == -1
    if error_occurred and not ignore_nonfatal:
        print("-- PASSED TEST --")
    else:
        print(f"{out}\n-- Test Failed --\n")
        if interactive:
            input("-- PRESS ENTER FOR NEXT TEST --")
        else:
            errors["ERROR_QUESTIONABLE_AS_GOOD"][0] += 1
            errors["ERROR_QUESTIONABLE_AS_GOOD"][1].append("questionable/" + file)

print("---- TEST 4 - CORRUPT BMPs ----")
valid_real_examples = os.listdir("./corrupt")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/corrupt/{file}\" {'--ignore-nonfatal' if ignore_nonfatal else ''}").read()
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