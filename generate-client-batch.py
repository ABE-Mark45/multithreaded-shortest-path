import argparse, random

parser = argparse.ArgumentParser('generator')
parser.add_argument('path')
parser.add_argument('--read-ratio', default=0.8, type=float)
parser.add_argument('--nodes', default=200, type=int)
parser.add_argument('--batches', default=10, type=int)
parser.add_argument('--ops', default=50, type=int)

if __name__ == '__main__':
    args = parser.parse_args()
    print(args)
    read_ratio = args.read_ratio
    nodes = args.nodes

    with open(args.path, 'w') as f:
        for _ in range(args.batches):
            for _ in range(args.ops):
                u, v = random.randint(0, nodes-1), random.randint(0, nodes-1)

                if random.random() < read_ratio:
                    f.write(f'Q {u} {v}\n')
                else:
                    if random.random() < 0.5:
                        f.write(f'A {u} {v}\n')
                    else:
                        f.write(f'D {u} {v}\n')
            f.write('F\n')
                        
                