import PYIM

def getAllIM():
    return [x for x in dir(PYIM) if "PYIM" in x and x[0] != "_" and x[-7:] != "Context"]
                    
if __name__ == '__main__':
    print getAllIM()
