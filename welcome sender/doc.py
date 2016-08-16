# -*- coding: utf-8 -*-

import subprocess
template_url = "welcome.tex"
work_url = "./"



def make_new(dic_field):
    template_file = open(template_url,'r')
    print dic_field
    new_file = open(work_url+dic_field["<<<name>>>"]+".tex","w")

    for line in template_file:
        new_line = line
        for key,value in dic_field.iteritems():
            new_line = new_line.replace(key,value)
        new_file.write(new_line)

    template_file.close()
    new_file.close()
    command = "pdflatex "+dic_field["<<<name>>>"]+".tex"
    print command
    cooktex = subprocess.Popen(command ,cwd=work_url,shell=True)
    cooktex.wait()
    return

