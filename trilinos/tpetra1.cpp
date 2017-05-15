#include <Tpetra_DefaultPlatform.hpp>
#include <Tpetra_Version.hpp>
#include <Teuchos_GlobalMPISession.hpp>
void
exampleRoutine (const Teuchos::RCP<const Teuchos::Comm<int> >& comm)
{
  if (comm->getRank () == 0) {
    std::cout << Tpetra::version () << std::endl << std::endl;
  }
}
int
main (int argc, char *argv[])
{
  using std::cout;
  using std::endl;
  Teuchos::GlobalMPISession mpiSession (&argc, &argv, NULL);
  Teuchos::RCP<const Teuchos::Comm<int> > comm =
    Tpetra::DefaultPlatform::getDefaultPlatform ().getComm ();
  const int myRank = comm->getRank ();
  const int numProcs = comm->getSize ();
  if (myRank == 0) {
    cout << "Total number of processes: " << numProcs << endl;
  }
  exampleRoutine (comm);
  if (myRank == 0) {
    cout << "End Result: TEST PASSED" << endl;
  }
  return 0;
}
