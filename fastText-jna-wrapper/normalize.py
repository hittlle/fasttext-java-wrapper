
texts = []
labels = []

with open("impolite-utf8.data", encoding="utf-8") as fp:
    texts = fp.readlines()

with open("pred.result", encoding="utf-8") as fp:
    labels = fp.readlines()

with open("impolite.data", "w", encoding="utf-8") as fp:
    count = len(texts)
    for idx in range(count):
        if "__label__neutral" not in labels[idx]:
            label = labels[idx][:-1]
            text = texts[idx][:-1]
            fp.write("%s %s\n" % (label, text))

print("[*]DONE")

