#include <sstream>
#include "parser.h"
#include "element.h"

Parser::Parser(ifstream& fin) {
   //string line;
   char line[1024] ;
   while(true) {
      fin.getline(line , 1024); // this can only use "char *" as argument
      if(fin.eof()) break;
      switch(line[0]) {
         case 'R':
         case 'L':
         case 'C':
            getRLC(line);
            break;
         case 'G':
            getGm(line);
            break;
         case 'V':
         case 'I':
         case 'O':
            getPreset(line);
            break;
         case 'F':
         case 'T':
            getConfig(line);
            break;
         default:
            // TODO: should raise error here
            break ;
      }
   }
}

void Parser::getRLC(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2;
   double value;
   Element *element;
   sin >> name >> n1 >> n2 >> value;
   // TODO: raise error

   switch(name[0]) {
      case 'R':
         element = new Resistor(name, value);
         break;
      case 'L':
         element = new Inductor(name, value);
         break;
      case 'C':
         element = new Capacitor(name, value);
         break;
      default:
         // TODO: raise error
         break ;
   }

   Node *node1 = circuit.getNodeById(n1), *node2 = circuit.getNodeById(n2);
   node1->setConnect(node2, (Element*)element->clone());
   node2->setConnect(node1, (Element*)element->clone());
   delete element ;
}

void Parser::getGm(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2, n3, n4;
   double value;
   sin >> name >> n1 >> n2 >> n3 >> n4 >> value;
   // TODO: raise error

   Element *element = new VCCS(name, value);
   Element *rev_element = new VCCS("R" + name, -value);

   Node *node1 = circuit.getNodeById(n1), *node2 = circuit.getNodeById(n2);
   Node *node3 = circuit.getNodeById(n3), *node4 = circuit.getNodeById(n4);

   node3->setConnect(node2, (Element *)element->clone());
   node3->setConnect(node1, (Element *)rev_element->clone());
   node4->setConnect(node1, (Element *)element->clone());
   node4->setConnect(node2, (Element *)rev_element->clone());
}


void Parser::getPreset(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2;
   sin >> name >> n1 >> n2;

   switch(name[0]) {
      case 'O':
         circuit.outputHighId = n1;
         circuit.outputLowId  = n2;
         break;
      case 'V':
         circuit.inputHighId = n1;
         circuit.inputLowId  = n2;
         circuit.inputType   = VIN;
         break;
      case 'I':
         circuit.inputHighId = n1;
         circuit.inputLowId  = n2;
         circuit.inputType  = IIN;
         break;
      default:
         // TODO: error
         break ;
   }
}

void Parser::getConfig(const string& line) {
   stringstream sin(line , stringstream::in);
   string type;
   SimulateConfig single;
   sin >> type >> single.start >> single.end >> single.step;

   switch(type[0]) {
      case 'F':
         single.type = FREQ;
         break;
      case 'T':
         single.type = TIME;
         break;
      default:
         // TODO: error
         break ;
   }
   config.push_back(single);
}
