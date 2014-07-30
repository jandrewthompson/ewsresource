#include "ewsresource.h"

#include "kmime/kmime_content.h"
#include "settings.h"
#include "settingsadaptor.h"
#include "kmime/kmime_util.h"
#include "kmime/kmime_message.h"
#include <akonadi/collectionfetchjob.h>
#include <QtDBus/QDBusConnection>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace Akonadi;
namespace fs = boost::filesystem;

ewsResource::ewsResource( const QString &id )
  : ResourceBase( id )
{
  new SettingsAdaptor( Settings::self() );
  QDBusConnection::sessionBus().registerObject( QLatin1String( "/Settings" ),
                            Settings::self(), QDBusConnection::ExportAdaptors );



  this->log("starting up...\n");


  // TODO: you can put any resource specific initialization code here.
}


ewsResource::~ewsResource()
{
}

//TODO: find a (better) logging framework
void ewsResource::log(string data)
{
	fs::ofstream fOut;

	  fOut.open("/tmp/log.out", std::ios::app);
	  if(!fOut)
	  {
			   cerr << "error loading file" << endl;
			   getchar();
			   exit(0);

	  }
	  fOut << data;

	  fOut.close();
}

void ewsResource::retrieveCollections()
{
  // TODO: this method is called when Akonadi wants to have all the
  // collections your resource provides.
  // Be sure to set the remote ID and the content MIME types
  
  this->log("retrieveCollections()\n");

  QStringList mimeTypes;
  mimeTypes << MAIL_MIME_TYPE;
  mimeTypes << Collection::mimeType();

  //Root Folder
  Collection collection;
  collection.setParentCollection(Collection::root());
  //remoteId :: just a unique QString.
  collection.setRemoteId( "2001" );
  collection.setRights( Collection::AllRights );
  collection.setContentMimeTypes( mimeTypes );
  collection.setName( "Bogus Mail" );
  

  //A Child folder
  Collection c2;
  c2.setParentCollection( collection );
  c2.setRemoteId( "2002" );
  c2.setRights( Collection::AllRights );
  c2.setContentMimeTypes( mimeTypes );
  c2.setName( "Other Mail" );

  //A sibling folder of c2
  Collection c3;
    c3.setParentCollection( collection );
    c3.setRemoteId( "2003" );
    c3.setRights( Collection::AllRights );
    c3.setContentMimeTypes( mimeTypes );
    c3.setName( "Junk Mail" );

  Collection::List collections;

  collections << collection;
  collections << c2;
  collections << c3;

  
  collectionsRetrieved( collections );
}

void ewsResource::retrieveItems( const Akonadi::Collection &collection )
{
  Q_UNUSED( collection );

  // TODO: this method is called when Akonadi wants to know about all the
  // items in the given collection. You can but don't have to provide all the
  // data for each item, remote ID and MIME type are enough at this stage.
  // Depending on how your resource accesses the data, there are several
  // different ways to tell Akonadi when you are done.
  
  this->log(
		  boost::str(
				  boost::format("Fetching Collection with name id: %s :: %s \n")
  	  	  	  	  	  % collection.name().toStdString()
  	  	  	  	  	  % collection.remoteId().toStdString()
    	  	  	  	  )
  );


  Item::List items;
  
  Item item;
  item.setMimeType( MAIL_MIME_TYPE );
  item.setRemoteId(  "1001" );
  
  Item item2;
  item2.setMimeType( MAIL_MIME_TYPE );
  item2.setRemoteId(  "1002" );
  
  //Pretending these two emails belong in different folders
  if(collection.remoteId() == "2002")
  {
	  items << item;

  } else if (collection.remoteId() == "2003")
  {
	  items << item2;
  }

  itemsRetrieved( items );
  
}

KMime::Message::Ptr buildMultiPartMessage()
{
  KMime::Message::Ptr m =  KMime::Message::Ptr( new KMime::Message );
  KMime::Headers::ContentType *ct = m->contentType();
  ct->setMimeType( "multipart/mixed" );
  ct->setBoundary( KMime::multiPartBoundary() );
  ct->setCategory( KMime::Headers::CCcontainer );
  m->contentTransferEncoding()->clear();
 
  // Set the headers.
  m->from()->fromUnicodeString( "some@mailaddy.com", "utf-8" );
  m->to()->fromUnicodeString( "someother@mailaddy.com", "utf-8" );
  m->cc()->fromUnicodeString( "some@mailaddy.com", "utf-8" );
  m->date()->setDateTime( KDateTime::currentLocalDateTime() );
  m->subject()->fromUnicodeString( "My Subject", "utf-8" );
  // Set the first multipart, the body message.
  KMime::Content *b = new KMime::Content;
  b->contentType()->setMimeType( "text/plain" );
  b->setBody( "Some text..." );
 
  KMime::Content *a = new KMime::Content;
  KMime::Headers::ContentDisposition *d = new KMime::Headers::ContentDisposition( a );
  d->setFilename( "cal.ics" );
  d->setDisposition( KMime::Headers::CDattachment );
  a->contentType()->setMimeType( "text/plain" );
  a->setHeader( d );
  a->setBody( "Some text in the attachment..." );
  
  // Attach the both multiparts and assemble the message.
  m->addContent( b );
  m->addContent( a );
  m->assemble(); 
 
  return m;
  
}

KMime::Message::Ptr buildPlainMessage()
{
  KMime::Message::Ptr m =  KMime::Message::Ptr( new KMime::Message );
  KMime::Headers::ContentType *ct = m->contentType();
  ct->setMimeType( "text/plain" );
  m->contentTransferEncoding()->clear();
 
  // Set the headers.
  m->from()->fromUnicodeString( "you@mailaddy.com", "utf-8" );
  m->to()->fromUnicodeString( "me@andrew.com", "utf-8" );
  m->cc()->fromUnicodeString( "me@andrew.com", "utf-8" );
  m->date()->setDateTime( KDateTime::currentLocalDateTime() );
  m->subject()->fromUnicodeString( "A plaing message subject", "utf-8" );
  // Set the first multipart, the body message.
  m->setBody("This is a plain message body");
 
  m->assemble();
  return m;
  
}

bool ewsResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( item );
  Q_UNUSED( parts );

  // TODO: this method is called when Akonadi wants more data for a given item.
  // You can only provide the parts that have been requested but you are allowed
  // to provide all in one go
  
  this->log("Fetching Item with remoteId: " + item.remoteId().toStdString() + "\n");

  Item newItem( item );
 
  if(item.remoteId() == "1001")
  {
    newItem.setPayload<KMime::Message::Ptr>( buildMultiPartMessage() );
  } else if(item.remoteId() == "1002") {
    newItem.setPayload<KMime::Message::Ptr>( buildPlainMessage() );
    
  }
  
  itemRetrieved( newItem );

  return true;
}


void ewsResource::aboutToQuit()
{
  // TODO: any cleanup you need to do while there is still an active
  // event loop. The resource will terminate after this method returns
}

void ewsResource::configure( WId windowId )
{
  Q_UNUSED( windowId );

  // TODO: this method is usually called when a new resource is being
  // added to the Akonadi setup. You can do any kind of user interaction here,
  // e.g. showing dialogs.
  // The given window ID is usually useful to get the correct
  // "on top of parent" behavior if the running window manager applies any kind
  // of focus stealing prevention technique
  //
  // If the configuration dialog has been accepted by the user by clicking Ok,
  // the signal configurationDialogAccepted() has to be emitted, otherwise, if
  // the user canceled the dialog, configurationDialogRejected() has to be emitted.
}

void ewsResource::itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection )
{
  Q_UNUSED( item );
  Q_UNUSED( collection );

  this->log("itemAdded()\n");
  // TODO: this method is called when somebody else, e.g. a client application,
  // has created an item in a collection managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

void ewsResource::itemChanged( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( item );
  Q_UNUSED( parts );

  this->log("itemChanged()\n");

  // TODO: this method is called when somebody else, e.g. a client application,
  // has changed an item managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

void ewsResource::itemRemoved( const Akonadi::Item &item )
{
  Q_UNUSED( item );

  this->log("itemRemoved()\n");
  // TODO: this method is called when somebody else, e.g. a client application,
  // has deleted an item managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

AKONADI_RESOURCE_MAIN( ewsResource )

#include "ewsresource.moc"
