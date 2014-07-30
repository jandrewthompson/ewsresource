#ifndef EWSRESOURCE_H
#define EWSRESOURCE_H
#include "kmime/kmime_content.h"
#include "kmime/kmime_util.h"
#include "kmime/kmime_message.h"
#include <stdio.h>
#include <akonadi/resourcebase.h>

class ewsResource : public Akonadi::ResourceBase,
                           public Akonadi::AgentBase::Observer
{
  Q_OBJECT

  public:
    ewsResource( const QString &id );
    ~ewsResource();

  public Q_SLOTS:
    virtual void configure( WId windowId );

  protected Q_SLOTS:
    void retrieveCollections();
    void retrieveItems( const Akonadi::Collection &col );
    bool retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts );
    

  protected:
    void log(std::string data);
    virtual void aboutToQuit();
    QString MAIL_MIME_TYPE = KMime::Message::mimeType();
    virtual void itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection );
    virtual void itemChanged( const Akonadi::Item &item, const QSet<QByteArray> &parts );
    virtual void itemRemoved( const Akonadi::Item &item );
    
};

#endif
