
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../../Headers/intermediatesteps/writefile.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Export;

namespace OpenGraphtheory
{
    namespace IntermediateSteps
    {

        FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerWriteFile::IntermediateStepHandlerWriteFileRegistrator(
            &IntermediateStepHandler::IntermediateStepHandlerFactory, "files", new DefaultInstantiator<IntermediateStepHandler, IntermediateStepHandlerWriteFile>(
                "files", "writes the graphs to files (using an export-filter, if present)",""));


        IntermediateStepHandlerWriteFile::IntermediateStepHandlerWriteFile()
        {
            index = 0;
            digits = 4;
            prefix = "opengt_";
            suffix = ".txt";
            exporter = NULL;
        }

        IntermediateStepHandlerWriteFile::~IntermediateStepHandlerWriteFile()
        {
            if(exporter != NULL)
                delete exporter;
        }

        void IntermediateStepHandlerWriteFile::Configure(vector<string> parameters)
        {
            if(parameters.size() > 0)
            {
                exporter = ExportFilter::ExportFilterFactory.Produce(parameters[0]);
                suffix = string(".") + parameters[0];
            }
        }

        void IntermediateStepHandlerWriteFile::Handle(Graph* G)
        {
            stringstream s;
            s << prefix << setfill('0') << setw(digits) << index++ << suffix;
            string filename;
            std::getline(s, filename);

            ofstream outdat(filename.c_str());

            if(exporter != NULL)
                exporter->Export(*G, outdat);
            else
                outdat << *G;

            outdat.close();
        }

    }
}
