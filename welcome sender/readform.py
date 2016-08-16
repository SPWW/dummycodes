# -*- coding: utf-8 -*-


import  csv


class formparser():
    def __init__(self):
        source_file = "input.csv"
        self.csvfile = open(source_file,"rb")
        self.spamreader = csv.reader(self.csvfile, delimiter=',', quotechar='|')

    def date2dic(self):
        ret = []
        title = True
        title_row = None
        for r in self.spamreader:
            if title is True:
                title_row = r
                title = False
            else:
                temp = {}
                for i in range(0,len(title_row)):
                    temp[title_row[i]] = r[i]
                ret.append(temp)
        return ret


if __name__ == '__main__':
    ff = formparser()
    print ff.csv2dic()