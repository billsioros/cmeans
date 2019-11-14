
import matplotlib.pyplot as plt

import sys

import argparse


argparser = argparse.ArgumentParser()

argparser.add_argument("-s", "--save")

args = argparser.parse_args()


xmin, xmax, ymin, ymax = float("+inf"), float("-inf"), float("+inf"), float("-inf")

colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

margin = 1

figure = plt.figure()

for index, line in enumerate(sys.stdin.readlines()):

    cluster = eval(line)

    plt.scatter(cluster[0][0], cluster[0][1], marker="x", color=colors[index % len(colors)])

    xs = [cluster[i][0] for i in range(1, len(cluster))]
    ys = [cluster[i][1] for i in range(1, len(cluster))]

    xmin, xmax = min(xs + [cluster[0][0], xmin]), max(xs + [cluster[0][0], xmax])
    ymin, ymax = min(ys + [cluster[0][1], ymin]), max(ys + [cluster[0][1], ymax])

    plt.scatter(xs, ys, marker="*", label=f"Cluster No.{index + 1}", color=colors[index % len(colors)])

plt.axis('equal')
plt.xlim([margin * xmin, margin * xmax])
plt.ylim([margin * ymin, margin * ymax])
plt.xlabel("$x$", color="#1C2833")
plt.ylabel("$y$", color="#1C2833")
plt.legend(loc="upper right")
plt.grid()
plt.show()

if args.save:

    figure.savefig(f"{args.save}", format="png")

