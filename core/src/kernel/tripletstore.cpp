#include <string>
#include <QString>
#include <QPair>
#include <QMultiHash>
#include <QMultiMap>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "tripletstore.h"

SOFT_BEGIN_NAMESPACE

class TripletStore :: Private
{
  friend class TripletStore;
  using TripletHash = QMultiHash<QString,QPair<QString,QString>>;
  using PredicateMap = QMultiMap<QString,QString>;
  PredicateMap predicateMap;
  TripletHash  triplets;
};

TripletStore :: TripletStore()
  : d(new TripletStore::Private())
{}

TripletStore :: ~TripletStore()
{
  delete d;
}

void TripletStore :: clear()
{
  d->triplets.clear();
}

void TripletStore :: addTriplet(std::string const &subj_, std::string const &pred_, std::string const &obj_)
{
  const QString &&pred = QString::fromStdString(pred_);
  const QString &&subj = QString::fromStdString(subj_);
  const QString &&obj  = QString::fromStdString(obj_);
  const QString &&rpred = "^"+pred;

  d->triplets.insert(subj, qMakePair<QString,QString>(pred, obj));
  d->triplets.insert(obj, qMakePair<QString,QString>(rpred, subj));
  Private::PredicateMap::const_iterator i = d->predicateMap.find(pred);
  while (i != d->predicateMap.cend() && i.key() == pred) {
    d->triplets.insert(obj, qMakePair<QString,QString>(i.value(), subj));
    ++i;
  }
}

///! Return all triplet pairs as CSV
//
std::string TripletStore :: toCSV() const
{
  QByteArray byteArray;
  QTextStream stream(&byteArray, QIODevice::WriteOnly);
  for (auto key: d->triplets.uniqueKeys()) {
    auto i = d->triplets.find(key);
    while (i != d->triplets.cend() && i.key() == key) {
      if (!i.value().first.startsWith("^")) {
        stream << key << "," << i.value().first << "," << i.value().second << endl;
      }
      ++i;
    }
  }
  return byteArray.toStdString();
}

// TODO: Should this be a constructor instead?
void TripletStore::fromCSV(std::string const &csv) {
  const QString c(csv.c_str());
  const QStringList relations = c.split("\n", QString::SkipEmptyParts);
  for (QString r: relations) {
    const QStringList triplet = r.split(",", QString::SkipEmptyParts);
    if (triplet.size() == 3) {
      const std::string subject = triplet[0].toStdString();
      const std::string predicate = triplet[1].toStdString();
      const std::string object = triplet[2].toStdString();
      addTriplet(subject, predicate, object);
    }
    else {
      throw std::runtime_error("Malformed triplet");
    }
  }
}

std::list<std::string> TripletStore :: findTriplets(const std::string &subj_, const std::string &pred_) const
{
  QString const &&subj = QString::fromStdString(subj_);
  QString const &&pred = QString::fromStdString(pred_);
  std::list<std::string>retval;

  Private::TripletHash::const_iterator i = d->triplets.find(subj);
  while (i != d->triplets.cend() && i.key() == subj) {
    if (pred.isNull() || pred == i.value().first) {
      retval.push_back(i.value().second.toStdString());
    }
    ++i;
  }
  return retval;
}

// Returns a vector of all subjects associated with a given predicate.
// For example:
//   subject predicate  object
//   Luke    child-of   Darth
// will return a vector containing "Luke" when called with findSubjectsWithPredicate("child-of")
std::vector<std::string> TripletStore::findSubjectsWithPredicate(std::string const &predicate) {
  const QString &&pred = QString::fromStdString(predicate);
  Private::PredicateMap::const_iterator i = d->predicateMap.find(pred);
  std::vector<std::string> results;
  while (i != d->predicateMap.cend() && i.key() == pred) {
    results.push_back(i.value().toStdString());
    ++i;
  }
  return results;
}

std::string TripletStore :: toRdf() const
{
  QByteArray byteArray;
  QXmlStreamWriter xml(&byteArray);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("rdf:RDF");
  xml.writeAttribute("xmlns:rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
  xml.writeAttribute("xmlns:soft", "http://www.sintef.no/rdf#");
  for (auto key: d->triplets.uniqueKeys()) {
    auto i = d->triplets.find(key);
    bool first = true;
    while (i != d->triplets.cend() && i.key() == key) {
      if (!i.value().first.startsWith("^")) {
  if (first) {
      xml.writeStartElement("rdf:Description");
      xml.writeAttribute("xmlns:about", key);
      first = !first;
  }
  xml.writeTextElement(i.value().first, i.value().second);
      }
      ++i;
    }
    if (!first)
      xml.writeEndElement();

  }
  xml.writeEndElement();
  xml.writeEndDocument();
  return byteArray.toStdString();
}


SOFT_END_NAMESPACE
