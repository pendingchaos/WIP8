import struct

class BlogEntry:
    def __init__(self, sourceFile, sourceLine, sourceFunction, action,
                 pointer, amount, type, typeSize, backtrace):
        self.sourceFile = sourceFile
        self.sourceLine = sourceLine
        self.sourceFunction = sourceFunction
        self.action = action
        self.pointer = pointer
        self.amount = amount
        self.type = type
        self.typeSize = typeSize
        self.backtrace = backtrace

class Blog:
    def __init__(self, filename):
        self.file = open(filename, "rb")
        self.pointerSize = 0
        self.entries = []
    
    def __del__(self):
        self.file.close()
    
    def read(self):
        self.entries = []
        
        self.file.seek(0)
        self.pointerSize = struct.unpack("<L", self.file.read(4))[0]
        
        if self.pointerSize == 4:
            sizeTFormat = "<L"
        else:
            sizeTFormat = "<Q"
        
        while True:
            action = self.file.read(4)
            
            if (action == ""):
                break;
            
            action = ["allocate", "deallocate", "new", "delete", "new array", "delete array"]\
                     [struct.unpack("<L", action)[0]]
            
            sourceFile = self._getCString(self.file)
            
            sourceLine = struct.unpack("<L", self.file.read(4))[0]
            
            sourceFunction = self._getCString(self.file)
            
            pointer = struct.unpack(sizeTFormat,
                                    self.file.read(self.pointerSize))[0]
            
            amount = struct.unpack(sizeTFormat,
                                   self.file.read(self.pointerSize))[0]
            
            type = self._getCString(self.file)
            
            typeSize = struct.unpack(sizeTFormat,
                                     self.file.read(self.pointerSize))[0]
            
            backtraceSize = struct.unpack("<L", self.file.read(4))[0]
            
            backtrace = []
            
            for i in xrange(backtraceSize):
                backtrace.append(self._getCString(self.file))
            
            self.entries.append(BlogEntry(sourceFile,
                                          sourceLine,
                                          sourceFunction,
                                          action,
                                          pointer,
                                          amount,
                                          type,
                                          typeSize,
                                          backtrace))

    def _getCString(self, file):
        string = ""
            
        while True:
            char = file.read(1)
                
            if (char == "\x00"):
                return string
                
            string += char
