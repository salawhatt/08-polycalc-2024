import os

def read_file(filename):
    with open(filename, 'r') as file:
        return file.read()

def main():
    files_to_read = ['main.c', 'lexer.l', 'parser.y', 'polycalc.h', 'polycalc.c']
    output_file = 'hm.txt'

    with open(output_file, 'w') as output:
        for filename in files_to_read:
            if os.path.exists(filename):
                output.write(f"--- Contents of {filename} ---\n")
                output.write(read_file(filename))
                output.write("\n\n")
            else:
                output.write(f"--- {filename} not found ---\n\n")

    print(f"Contents of specified files have been written to {output_file}")

if __name__ == "__main__":
    main()

