#ifndef QENTITY_H_DEF
#define QENTITY_H_DEF

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "genericentity.h"

class QJsonDocument;
class QEntity : public QObject

{
  Q_OBJECT
public:
  explicit QEntity(QObject *parent = nullptr);
  explicit QEntity(QEntity const &other);
  QEntity(QString const &id, QObject *parent = nullptr);  
  virtual ~QEntity();
  
  QSharedPointer<soft::GenericEntity> generic;  
public slots:
  void setSchema(QString const&);
  void debug() const;
  QString id() const;
  QString metaName() const;
  QString metaNamespace() const;
  QString metaVersion() const;
  QVariant property(QString const&) const;
  void setProperty(QString const&, QVariant const&);
};

#endif // QENTITY_H_DEF
