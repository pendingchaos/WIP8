import blog

blogFile = blog.Blog("../bin/memory.blog")

blogFile.read()

print "Pointer size: %d" % (blogFile.pointerSize)

for entry in blogFile.entries:
    print "%s:%d:%s action:%s pointer:%x amount:%d type:%s type size:%d" %\
    (entry.sourceFile, entry.sourceLine, entry.sourceFunction, entry.action, entry.pointer, entry.amount, entry.type, entry.typeSize)
    
    for frame in entry.backtrace:
        print "    %s" % (frame)
