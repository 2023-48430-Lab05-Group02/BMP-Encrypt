import subprocess, os

print("---- TEST 1 - REAL EXAMPLES BMPs ----")
valid_real_examples = os.listdir("./valid-examples")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/valid-examples/{file}\"").read()
    if out.find("Successfully read bmp") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
        print("-- PASSED TEST --")
    else:
        print(f"-- Test Failed --\n{out}")
        input("-- PRESS ENTER FOR NEXT TEST --")

print("---- TEST 2 - TEST CASE BMPs ----")
valid_real_examples = os.listdir("./valid")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/valid/{file}\"").read()
    if out.find("Successfully read bmp") != -1 and out.find("An error has occurred reading the BMP Data") == -1:
        print("-- PASSED TEST --")
    else:
        print(f"-- Test Failed --\n{out}")
        input("-- PRESS ENTER FOR NEXT TEST --")


print("---- TEST 3 - QUESTIONABLE BMPs ----")
valid_real_examples = os.listdir("./questionable")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/questionable/{file}\"").read()
    if out.find("An error has occurred reading the BMP Data") != -1 and out.find("Successfully read bmp") == -1:
        print("-- PASSED TEST --")
    else:
        print(f"-- Test Failed --\n{out}")
        input("-- PRESS ENTER FOR NEXT TEST --")

print("---- TEST 4 - CORRUPT BMPs ----")
valid_real_examples = os.listdir("./corrupt")
for file in valid_real_examples:
    print(f"-- TEST FILE: {file} --")
    out = os.popen(f"..\\cmake-build-debug-msys2\\untitled.exe --encrypt --input \"../test/corrupt/{file}\"").read()
    if out.find("An error has occurred reading the BMP Data") != -1 and out.find("Successfully read bmp") == -1:
        print("-- PASSED TEST --")
    else:
        print(f"-- Test Failed --\n{out}")
        input("-- PRESS ENTER FOR NEXT TEST --")
