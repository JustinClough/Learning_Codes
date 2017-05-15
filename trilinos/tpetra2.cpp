#include <Tpetra_DefaultPlatform.hpp>
#include <Tpetra_Vector.hpp>
#include <Tpetra_Version.hpp>
#include <Teuchos_GlobalMPISession.hpp>
#include <Teuchos_oblackholestream.hpp>
void
exampleRoutine (const Teuchos::RCP<const Teuchos::Comm<int> >& comm,
                std::ostream& out)
{
  using std::endl;
  using Teuchos::Array;
  using Teuchos::ArrayView;
  using Teuchos::RCP;
  using Teuchos::rcp;
  out << Tpetra::version () << endl << endl;
  typedef Tpetra::Vector<>::scalar_type scalar_type;
  typedef Tpetra::Vector<>::global_ordinal_type global_ordinal_type;
  typedef Tpetra::Map<> map_type;
  const size_t numLocalEntries = 5;
  const Tpetra::global_size_t numGlobalEntries =
    comm->getSize () * numLocalEntries;
  const global_ordinal_type indexBase = 0;
  RCP<const map_type> contigMap =
    rcp (new map_type (numGlobalEntries, indexBase, comm));
  TEUCHOS_TEST_FOR_EXCEPTION(
    ! contigMap->isContiguous (), std::logic_error,
    "The supposedly contiguous Map isn't contiguous.");
  RCP<const map_type> contigMap2 =
    rcp (new map_type (numGlobalEntries, numLocalEntries, indexBase, comm));
  TEUCHOS_TEST_FOR_EXCEPTION(
    ! contigMap->isSameAs (*contigMap2), std::logic_error,
    "contigMap should be the same as contigMap2, but it's not.");
  const Tpetra::global_size_t INVALID =
    Teuchos::OrdinalTraits<Tpetra::global_size_t>::invalid ();
  RCP<const map_type> contigMap3 =
    rcp (new map_type (INVALID, numLocalEntries, indexBase, comm));
  TEUCHOS_TEST_FOR_EXCEPTION(
    ! contigMap2->isSameAs (*contigMap3), std::logic_error,
    "contigMap2 should be the same as contigMap3, but it's not.");
  RCP<const map_type> cyclicMap;
  {
    Array<global_ordinal_type>::size_type numEltsPerProc = 5;
    Array<global_ordinal_type> elementList (numEltsPerProc);
    const int numProcs = comm->getSize ();
    const int myRank = comm->getRank ();
    for (Array<global_ordinal_type>::size_type k = 0; k < numEltsPerProc; ++k) {
      elementList[k] = myRank + k*numProcs;
    }
    cyclicMap = rcp (new map_type (numGlobalEntries, elementList, indexBase, comm));
  }
  TEUCHOS_TEST_FOR_EXCEPTION(
    comm->getSize () > 1 && cyclicMap->isContiguous (),
    std::logic_error,
    "The cyclic Map claims to be contiguous.");
  TEUCHOS_TEST_FOR_EXCEPTION(! contigMap->isCompatible (*cyclicMap),
    std::logic_error,
    "contigMap should be compatible with cyclicMap, but it's not.");
  TEUCHOS_TEST_FOR_EXCEPTION(comm->getSize() > 1 && contigMap->isSameAs (*cyclicMap),
    std::logic_error,
    "contigMap should be compatible with cyclicMap, but it's not.");
  typedef Tpetra::Vector<> vector_type;
  vector_type x (contigMap);
  vector_type y (x, Teuchos::Copy);
  vector_type z (contigMap, false);
  z.randomize ();
  x.putScalar (1.0);
  const scalar_type alpha = 3.14159;
  const scalar_type beta = 2.71828;
  const scalar_type gamma = -10.0;
  x.update (alpha, z, beta);
  y.putScalar (42.0);
  y.update (alpha, x, beta, z, gamma);
  typedef Tpetra::Vector<>::mag_type mag_type;
  const mag_type theNorm = y.norm2 ();
  out << "Norm of y: " << theNorm << endl;
}
int
main (int argc, char *argv[])
{
  using std::endl;
  using Teuchos::RCP;
  Teuchos::oblackholestream blackHole;
  Teuchos::GlobalMPISession mpiSession (&argc, &argv, &blackHole);
  RCP<const Teuchos::Comm<int> > comm =
    Tpetra::DefaultPlatform::getDefaultPlatform ().getComm ();
  const int myRank = comm->getRank ();
  std::ostream& out = (myRank == 0) ? std::cout : blackHole;
  exampleRoutine (comm, out);
  if (myRank == 0) {
    std::cout << "End Result: TEST PASSED" << std::endl;
  }
  return 0;
}
