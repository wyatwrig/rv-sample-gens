import argparse
import random
from pathlib import Path
import shutil


class MalwareGenerator:

    def __init__(self, intype, count, sec, junk, wait):
        self.sampleTrack = 0
        self.sampleCount = count
        self.sampleType = intype
        self.sampleSeconds = sec
        self.sampleJunk = junk
        self.sampleWait = wait

        self.cleanActions = ["openPort();", "readFile();", "writeFile();", "deleteFile();"] # reverse to function as stack
        self.malActions = ["createService();", "changeProcName();", "openPort();", "ransomFiles();", "clearLogs();"]
        self.malActions.reverse()
        self.cleanActions.reverse()

    # obfuscation functions wait();, math();
    def obfuscate(self):
        insert = self.sampleJunk + self.sampleWait
        obfArray = []
        if (insert > 0):
            junkTemp = self.sampleJunk
            waitTemp = self.sampleWait
            while (waitTemp > 0 and junkTemp > 0):
                flip = random.randint(0,1)
                if flip == 0:
                    obfArray.append(f"wait({self.sampleSeconds});")
                    waitTemp -= 1
                if flip == 1:
                    obfArray.append("math();")
                    junkTemp -= 1
        return obfArray
    
    def copyTemplate(self):
        path = Path(__file__).parent.resolve()
        newFile = str(path) + f"/{self.sampleType}_{self.sampleCount}.cpp"
        shutil.copyfile(str(path) + "/malware.cpp", newFile)
        return newFile


    def writeToFile(self):
        filename = self.copyTemplate()
        payload = []
        if self.sampleType == 'malicious':
            payload = self.malActions
        if self.sampleType == 'nonmalicious':
            payload = self.cleanActions
        with open(filename, 'a') as f:
            f.write("\n\nint main(int argc, char **argv){\n")
            while payload:
                f.write(f"\t{payload.pop()}\n")
                obfsArray = self.obfuscate()
                while obfsArray:
                    f.write(f"\t{obfsArray.pop()}\n")
            f.write("}")

    def start(self):
        while self.sampleTrack != self.sampleCount:
            self.writeToFile()
            self.sampleTrack += 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="MalwareGenerator",
        description="A CLI to generate malicious and nonmalicious malware samples"
    )
    parser.add_argument('type', choices=['malicious', 'nonmalicious'], help="Determines type of sample to generate. Options are \'malicious\' and \'nonmalicious\'.", metavar='type')
    parser.add_argument('-n', '--number', default=1, help="Sets the number of samples to be generated", type=int)
    parser.add_argument('-s', '--seconds', default=5, help="Sets the time in seconds used by the wait function", type=int)
    parser.add_argument('-j', '--junk', default=0, help="Sets the number of junk math functions to add between actions", type=int)
    parser.add_argument('-w','--wait', default=0, help="Sets the number of wait functions between actions", type=int)
    args = parser.parse_args()

    malgen = MalwareGenerator(args.type, args.number, args.seconds, args.junk, args.wait)
    malgen.start()


