import blog

blogFile = blog.Blog("../bin/memory.blog")

blogFile.read()

allocations = {}

for entry in blogFile.entries:
    if entry.action in ["allocate", "new", "new array"]:
        allocations[entry.pointer] = entry
    else:
        if allocations.has_key(entry.pointer):
            del allocations[entry.pointer]
        else:
            print "Freeing an invalid pointer. Pointer: %x Location: %s:%d:%s" %\
            (entry.pointer, entry.sourceFile, entry.sourceLine, entry.sourceFunction)

for pointer, entry in allocations.items():
    print "Leaked memory. Pointer: %x Location: %s:%d:%s" %\
    (entry.pointer, entry.sourceFile, entry.sourceLine, entry.sourceFunction)
