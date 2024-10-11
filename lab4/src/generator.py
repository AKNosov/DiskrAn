import random

def generate_pattern_and_lines_with_periodic_pattern(file_name):
    max_uint32 = 2**32 - 1

    pattern_length = random.randint(2, 10)
    pattern = [random.randint(0, max_uint32) for _ in range(pattern_length)]

    num_lines = random.randint(15, 100) 
    lines = []

    period = random.randint(2, 5)  

    for i in range(num_lines):
        if i % period == 0:
            line_length = random.randint(len(pattern), len(pattern) + 50)
            line = [random.randint(0, max_uint32) for _ in range(line_length - len(pattern))]
            insert_position = random.randint(0, len(line))
            line = line[:insert_position] + pattern + line[insert_position:]
        else:
            line_length = random.randint(50, 100)
            line = [random.randint(0, max_uint32) for _ in range(line_length)]
        lines.append(line)

    with open(file_name, 'w') as f:
        f.write(' '.join(map(str, pattern)) + '\n')
        for line in lines:
            f.write(' '.join(map(str, line)) + '\n')

generate_pattern_and_lines_with_periodic_pattern('1.txt')
