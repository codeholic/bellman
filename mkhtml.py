import sys, os, glob
import subprocess

# print "<html><body>"

# print "<table border=1>"

# ctr = 1

# try:
#     while 1:
#         name_base = "result" + ("000000" + str(ctr))[-6:]

#         os.stat(name_base + "-4.out")

#         print "<tr>"

#         print "<td><img src='" + name_base + "-4-partial.gif'></td>"
#         #print "<td><img src='" + name_base + "-4-full.gif'></td>"
        
#         print "</tr>"
     

#         ctr += 1
# except OSError:
#     pass

# print "</table>"
# print "</body></html>"

class Category:
    def __init__(self, category):
        self.category = category
        self.results = []

    def print_html(self, file):
        print >> file, "<html><body><table border=1>"
        is_first = True
        
        for r in self.results:
            r, log = r
            rr = r.split("/")[-1]
            rr = rr.split(".")[0]
            print >> file, "<tr>"
            print >> file, "<td>", rr, "</td>"
            print >> file, "<td><pre>" + log + "</pre></td>"
            print >> file, "<td><img src='" + rr + "-partial.gif'></td>"
            if is_first:
                is_first = False
                print >> file, "<td><img src='" + rr + "-full.gif'></td>"
            print >> file, "</tr>"
        print >> file, "</table></body></html>"

def print_link(file, href, text):
    print >> file, "<a href='" + href + "'>" + text + "</a>"
                   
def main():
    path = sys.argv[1]
    files = glob.glob(os.path.join(path, "*.out"))

    cats = {}

    for f in files:
        lines = subprocess.Popen(["./bellman", "-c", f], stdout=subprocess.PIPE).communicate()[0]
        lines = lines.split("\n")
        d = {}
        for l in lines:
            idx = l.find(":")
            if idx > 0:
                key = l[:idx].strip()
                val = l[idx+1:].strip()
                if d.has_key(key):
                    prev = d[key] + "\n"
                else:
                    prev = ""
                d[key] = prev + val
        category = d["hash"]
        if not cats.has_key(category):
            cats[category] = Category(category)
        cats[category].results.append((f, d["log"]))

    for c in cats.keys():
        f = open(os.path.join(path, "category-" + c + ".html"), "w")
        cats[c].print_html(f)
        f.close()

    f = open(os.path.join(path, "index.html"), "w")
    print >> f, "<html><body>"

    print_link(f, ".", "All files")
    
    print >> f, "<table border=1>"
    for c in cats.keys():
        cc = cats[c]
        fname, log = cc.results[0]
        rr = fname.split("/")[-1]
        rr = rr.split(".")[0]
        print >> f, "<tr>"
        print >> f, "<td><a href=category-" + c + ".html>", len(cc.results), "results", "</a></td>"
        print >> f, "<td><pre>" + log + "</pre></td>"
        print >> f, "<td><img src='" + rr + "-partial.gif'></td>"
        print >> f, "</tr>"

    print >> f, "</table></body></html>"
    f.close()
    
if __name__ == "__main__":
    main()
