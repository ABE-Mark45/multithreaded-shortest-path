import argparse, random

parser = argparse.ArgumentParser('generator')
parser.add_argument('path')
parser.add_argument('--nodes', default=200, type=int)
parser.add_argument('--edges-ratio', default=0.1, type=float)

if __name__ == '__main__':
    args = parser.parse_args()
    nodes = args.nodes
    edges_ratio = args.edges_ratio

    edge_count = int(nodes * (nodes - 1) * edges_ratio)

    with open(args.path, 'w') as f:
        for _ in range(edge_count):
            u, v = random.randint(0, nodes-1), random.randint(0, nodes-1)
            f.write(f'{u} {v}\n')
        f.write('S\n')
                        
                