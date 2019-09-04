import argparse
import random
import json
import sys

def parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("--length", type=int, help="length of generated text")
    parser.add_argument("--model", type=str, help="path to model")
    parser.add_argument("--seed", type=str, default = '', help="word to start with")
    parser.add_argument("--output", type=str, default = 'stdout', help="path to output file")
    return parser.parse_args()
    
def generateText(seed, length, frequency_dict):
    text = list()
    #Составляем список слов, с которых может начинаться наш текст
    startlist = list(frequency_dict.keys())

    if seed != '':
        curword = seed
    else:
        curword = random.choice(startlist)
    
    text.append(curword)
    for _ in range(length-1):
        l = list()
        for word in frequency_dict[curword].keys():
            l += [word] * frequency_dict[curword][word]
        curword = random.choice(l)
        #если наткнулись на конец текста, то рандомно выбираем следующее слово
        if curword == '#':
            curword = random.choice(startlist)
        text.append(curword)
    return text
    
def main():

    #парсим

    args = parse()

    try:
        fin = open(args.model, 'r', encoding="utf-8")
    except IOError:
        print("incorrect model file path")
    if args.output != 'stdout':
        try:
            output_stream = open(args.output, 'w', encoding="utf-8")
        except IOError:
            print("incorrect output file path")
    else:
        output_stream = sys.stdout
    r = fin.read()
    #восстанавливаем по файлу нашу модель
    frequency_dict = json.loads(r)
    
    output = ' '.join(generateText(args.seed, args.length, frequency_dict))
    output_stream.write(output)
    output_stream.close()
    fin.close()
    
if __name__ == "__main__":
    main()
