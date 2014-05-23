
#include "../../../Headers/algorithms/treewidth/treewidth.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace Treewidth
        {

            NiceTreeDecomposition::NiceTreeDecomposition()
            {
            }

            NiceTreeDecomposition::~NiceTreeDecomposition()
            {
            }

            // -------------------------------------------------------------------------------------------

            IntroduceNode::IntroduceNode(NiceTreeDecomposition* Subtree, VertexIterator IntroducedNode)
                : NiceTreeDecomposition()
            {
                this->IntroducedNode = IntroducedNode;
                this->Subtree = Subtree;
            }

            IntroduceNode::~IntroduceNode()
            {
                if(Subtree != NULL)
                    delete Subtree;
            }

            int IntroduceNode::Width()
            {
                if(Subtree == NULL)
                    return 1;

                int a = Subtree->Width();
                int b = this->BagSize();
                return a>b?a:b;
            }

            int IntroduceNode::BagSize()
            {
                if(Subtree == NULL)
                    return 1;
                return Subtree->BagSize() + 1;
            }

            // -------------------------------------------------------------------------------------------

            ForgetNode::ForgetNode(NiceTreeDecomposition* Subtree, VertexIterator ForgottenNode)
                : NiceTreeDecomposition()
            {
                this->ForgottenNode = ForgottenNode;
                this->Subtree = Subtree;
            }

            ForgetNode::~ForgetNode()
            {
                delete Subtree;
            }

            int ForgetNode::Width()
            {
                return Subtree->Width();
            }

            int ForgetNode::BagSize()
            {
                return Subtree->BagSize() - 1;
            }

            // -------------------------------------------------------------------------------------------

            JoinNode::JoinNode(NiceTreeDecomposition* Subtree1, NiceTreeDecomposition* Subtree2)
                : NiceTreeDecomposition()
            {
                this->Subtree1 = Subtree1;
                this->Subtree2 = Subtree2;
            }

            JoinNode::~JoinNode()
            {
                delete Subtree1;
                delete Subtree2;
            }

            int JoinNode::Width()
            {
                int a = Subtree1->Width();
                int b = Subtree2->Width();
                return a>b ? a : b;
            }

            int JoinNode::BagSize()
            {
                return Subtree1->BagSize();
            }


        }
    }
}
