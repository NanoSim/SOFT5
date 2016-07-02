#ifndef QSTORAGE_H_DEF
#define QSTORAGE_H_DEF

#include <QObject>
#include <QString>
#include <Soft>

class QEntity;
class QStorage : public QObject
{
  Q_OBJECT
public:
  QStorage(QString const &driver,
	   QString const &uri,
	   QString const &options = QString());
  virtual ~QStorage();

public slots:
  void save (QEntity const *);
  void load (QEntity *);

private:
  QScopedPointer<soft::Storage> storage;
};


#endif // QSTORAGE_H_DEF
