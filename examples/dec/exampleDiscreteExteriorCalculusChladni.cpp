#include <sstream>
#include <string>
using namespace std;

#include "DGtal/math/linalg/EigenSupport.h"
#include "DGtal/dec/DiscreteExteriorCalculus.h"
#include "DGtal/dec/DiscreteExteriorCalculusSolver.h"

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/math/linalg/EigenSupport.h"
#include "DGtal/dec/DiscreteExteriorCalculus.h"
#include "DGtal/dec/DiscreteExteriorCalculusSolver.h"
using namespace DGtal;

#include <Eigen/Eigenvalues>

double standard_deviation(const Eigen::VectorXd& xx)
{
    const double mean = xx.mean();
    double accum = 0;
    for (int kk=0, kk_max=xx.rows(); kk<kk_max; kk++)
        accum += (xx(kk)-mean)*(xx(kk)-mean);
    return sqrt(accum)/(xx.size()-1);
}

int main(int argc, char* argv[])
{
    trace.beginBlock("building calculus");

    const Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(10,10));

    typedef DiscreteExteriorCalculus<Z2i::Domain, EigenSparseLinearAlgebraBackend> Calculus;
    Calculus calculus(domain);

    // bottom linear structure
    // left and right Dirichlet boundary condition
    for (int kk=2; kk<=20; kk++)
        calculus.insertSCell( calculus.myKSpace.sCell(Z2i::Point(kk, 1)) );

    // top linear structure
    // left Neumann boundary condition, right Dirichlet boundary condition
    for (int kk=3; kk<=20; kk++)
        calculus.insertSCell( calculus.myKSpace.sCell(Z2i::Point(kk, 21)) );

    for (int kk=3; kk<20; kk++)
        for (int ll=3; ll<20; ll++)
        {
            if (kk==11 && ll==11) continue;
            calculus.insertSCell( calculus.myKSpace.sCell(Z2i::Point(kk, ll)) );
        }

    trace.info() << calculus << endl;

    trace.endBlock();

    trace.beginBlock("building laplace");

    Calculus::DualIdentity0 laplace = -1 * calculus.dualLaplace();
    trace.info() << "laplace=" << laplace << endl;

    {
        Board2D board;
        board << domain;
        board << calculus;
        board.saveSVG("chladni_calculus.svg");
    }

    trace.endBlock();

    trace.beginBlock("finding laplace eigen vectors");

    typedef Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> EigenSolverMatrix;
    const EigenSolverMatrix eigen_solver(laplace.myContainer);

    const Eigen::VectorXd eigen_values = eigen_solver.eigenvalues();
    const Eigen::MatrixXd eigen_vectors = eigen_solver.eigenvectors();
    for (int kk=0; kk<laplace.myContainer.rows(); kk++)
    {
        Calculus::Scalar eigen_value = eigen_values(kk, 0);

        const Eigen::VectorXd eigen_vector = eigen_vectors.col(kk);
        const Calculus::DualForm0 eigen_form = Calculus::DualForm0(calculus, eigen_vector);
        std::stringstream ss;
        ss << "chladni_eigen_" << kk << ".svg";
        const std::string filename = ss.str();
        ss << "chladni_eigen_vector_" << kk << ".svg";
        trace.info() << kk << " " << eigen_value << " " << sqrt(eigen_value) << " " << eigen_vector.minCoeff() << " " << eigen_vector.maxCoeff() << " " << standard_deviation(eigen_vector) << endl;

        Board2D board;
        board << domain;
        board << calculus;
        board << CustomStyle("KForm", new KFormStyle2D(eigen_vectors.minCoeff(),eigen_vectors.maxCoeff()));
        board << eigen_form;
        board.saveSVG(filename.c_str());
    }

    trace.endBlock();

    return 0;
}

