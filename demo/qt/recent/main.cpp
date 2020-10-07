#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDomDocument>
#include <QXmlStreamWriter>

class RecentDom
{
public:
    RecentDom (QString path)
    {
        xmlPath = path;
    }

    bool read ()
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        QFile file (xmlPath);
        if (!file.exists()) {
            return false;
        }

        file.open(QIODevice::ReadOnly | QIODevice::Text);

        if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
            qDebug() << "line:" << errorLine << " column:" << errorColumn << " info:" << errorStr;
            return false;
        }

        QDomElement root = domDocument.documentElement();
        if ("xbel" != root.tagName()) {
            qDebug () << "not xbel document";
            return false;
        } else if (root.hasAttribute("version") && "1.0" != root.attribute("version")) {
            qDebug() << "xbel version is not 1.0";
            return false;
        }

        qDebug() << domDocument.toString();
        return true;
    }

    bool write ()
    {
        QFile file (xmlPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

        file.write(domDocument.toByteArray());

        file.flush();
        file.close();

        return true;
    }

    bool elementExists (QString uri)
    {
        return false;
    }

    bool createNode ()
    {
        QDomElement rootElement = domDocument.firstChildElement();
        rootElement.setAttribute("version", "1.0");
        rootElement.setAttribute("xmlns:bookmark", "http://www.freedesktop.org/standards/desktop-bookmarks");
        rootElement.setAttribute("xmlns:mime", "http://www.freedesktop.org/standards/shared-mime-info");

        QDomElement child = domDocument.createElement("bookmark");
        child.setAttribute("href", "file:///home/dingjing/a");
        child.setAttribute("added", "2020-09-04T07:52:25Z");
        child.setAttribute("modified", "2020-09-04T07:52:25Z");
        child.setAttribute("visited", "2020-09-04T07:52:25Z");

        QDomElement childInfo = domDocument.createElement("info");

        QDomElement childInfoMeta = domDocument.createElement("metadata");
        childInfoMeta.setAttribute("owner", "http://freedesktop.org");

        QDomElement childInfoMetaMime = domDocument.createElement("mime:mime-type");
        childInfoMetaMime.setAttribute("type", "application/x-alpm-package");

        QDomElement childInfoMetaApp = domDocument.createElement("bookmark:applications");

        // not only one application
        QDomElement app = domDocument.createElement("bookmark:application");
        app.setAttribute("name", "Firefox");
        app.setAttribute("exec", "&apos;firefox %u&apos;");
        app.setAttribute("modified", "2020-09-14T11:19:05Z");
        app.setAttribute("count", "1");


        childInfoMetaApp.appendChild(app);
        childInfoMeta.appendChild(childInfoMetaMime);
        childInfoMeta.appendChild(childInfoMetaApp);
        childInfo.appendChild(childInfoMeta);
        child.appendChild(childInfo);
        rootElement.appendChild(child);

        return true;
    }

private:
    QString xmlPath;
    QDomDocument domDocument;
};




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString path = "/home/dingjing/.local/share/recently-used.xbel";

    RecentDom dom(path);

    dom.read();
    dom.createNode();
    dom.write();


    return a.exec();
}
