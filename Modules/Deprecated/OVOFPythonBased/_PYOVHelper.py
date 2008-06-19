import PYOVFilter

def getAllFilters():
    return [x for x in dir(PYOVFilter) if "PYOVFilter" in x and x[0] != "_"]
                    
if __name__ == '__main__':
    print getAllFilters()