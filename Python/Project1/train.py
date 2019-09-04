import sys
import argparse
import re
import json
import glob
from collections import defaultdict
    
def parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", type=str, default = 'stdin', help="path to input files")
    parser.add_argument("--model", type=str, help="path to model file")
    parser.add_argument("--lc", action="store_true", help="lower the text")
    args = parser.parse_args()
    return args
    
def splitIntoWords(line, lower):
    deviders = '[ \,\.\!-\?\;\:\"\'\n]'
    if lower:
        lst = re.split(deviders, line.lower())
    else:
        lst = re.split(deviders, line)
    words = list()
    for x in lst:
        if x != '':
            words.append(x)
    return words
    
def treatInputFile(input_stream, frequency_dict, lastword, lower):
    for line in input_stream.readlines():
        if line == '--END--':
            break
        #преобразуем строку
        wordlist = splitIntoWords(line, lower)
        if lastword != '' and len(wordlist) > 0:
            frequency_dict[lastword][wordlist[0]] += 1
        for i in range(len(wordlist) - 1):
            word1 = wordlist[i]
            word2 = wordlist[i+1]
            frequency_dict[word1][word2] += 1
            lastword = word2

def main():
    frequency_dict = defaultdict(lambda: defaultdict(int))

    #парсим
    args = parse()
    
    filelist = list()
    
    if args.input != 'stdin':
        filelist = glob.glob(args.input + "/*")
    else:
        filelist.append(sys.stdin)
    
    try:
        fout = open(args.model, 'w', encoding="utf-8")
    except IOError:
                print("incorrect model file path")
    
    for path in filelist:
        lastword = ''
        
        if path != 'stdin':
            try:
                input_stream = open(path, 'r', encoding="utf-8")
            except IOError:
                print("incorrect input file path")
        else:
            input_stream = sys.stdin
            
        treatInputFile(input_stream, frequency_dict, lastword, args.lc)
            
        frequency_dict[lastword]['#'] = 1 #символ конца файла
            
    #записываем в выходной файл
    r = json.dumps(frequency_dict)
    fout.write(r)

    input_stream.close()
    fout.close()
    
if __name__ == "__main__":
    main()
