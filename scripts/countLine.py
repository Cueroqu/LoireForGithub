import re
import subprocess

def count_code_line():
    test = subprocess.Popen(["wc -l ../Classes/*"], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cnt1 = int(re.search('\d+', test.communicate()[0].strip("\n").split("\n", -1)[-1]).group(0))
    test = subprocess.Popen(["wc -l ../Classes/Arsenal/*"], shell=True, stdout=subprocess.PIPE)
    cnt1 += int(re.search('\d+', test.communicate()[0].strip("\n").split("\n", -1)[-1]).group(0))
    print "code line: {0} lines".format(cnt1)

if __name__ == "__main__" :
    count_code_line()
#    count_config_line()
