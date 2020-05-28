#pragma once 

#include <QApplication>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>

class proxyModel : public QSortFilterProxyModel
{
public:
    proxyModel( const QString& targetDir ) : dir( targetDir )
    {
        if ( !dir.endsWith( "/" ) )
        {
            dir += "/";
        }
    }

protected:
    virtual bool filterAcceptsRow( int source_row
                                 , const QModelIndex & source_parent ) const
    {
        QString path;
        QModelIndex pathIndex = source_parent.child( source_row, 0 );
        while ( pathIndex.parent().isValid() )
        {
            path = sourceModel()->data( pathIndex ).toString() + "/" + path;
            pathIndex = pathIndex.parent();
        }
        // Get the leading "/" on Linux. Drive on Windows?
        path = sourceModel()->data( pathIndex ).toString() + path;

        // First test matches paths before we've reached the target directory.
        // Second test matches paths after we've passed the target directory.
        return dir.startsWith( path ) || path.startsWith( dir );
    }

private:
    QString dir;
};
