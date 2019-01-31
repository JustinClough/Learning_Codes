
def list_benefits():
    benefits = [    
        "More organized code",
        "More readable code",
        "Easier code reuse",
        "Allowing programmers to share and connect code together"]
    return benefits

def build_sentence( benefit):
    trailer = " is a benefit of functions!"
    statement = benefit + trailer
    return statement

def name_the_benefits_of_functions():
    list_of_benefits = list_benefits()

    for benefit in list_of_benefits:
        print(build_sentence( benefit))

name_the_benefits_of_functions()
