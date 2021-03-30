//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_ICON_INFO_P_H
#define LIBCORE_DINGJING_ICON_INFO_P_H
#include <QPainter>
#include <gio/gio.h>
#include <QIconEngine>

#include "icon-info.h"

namespace dingjing {

    class IconEngine: public QIconEngine
    {
    public:
        IconEngine(std::shared_ptr<const dingjing::IconInfo> info);

        ~IconEngine() override;

        QSize actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

        // not supported
        void addFile(const QString& /*fileName*/, const QSize& /*size*/, QIcon::Mode /*mode*/, QIcon::State /*state*/) override {}

        // not supported
        void addPixmap(const QPixmap& /*pixmap*/, QIcon::Mode /*mode*/, QIcon::State /*state*/) override {}

        QIconEngine* clone() const override;

        QString key() const override;

        void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;

        QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

        void virtual_hook(int id, void* data) override;

    private:
        std::weak_ptr<const dingjing::IconInfo> info_;
    };

    IconEngine::IconEngine(std::shared_ptr<const IconInfo> info): info_{info} {
    }

    IconEngine::~IconEngine() {
    }

    QSize IconEngine::actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) {
        auto info = info_.lock();
        return info ? info->internalQicon().actualSize(size, mode, state) : QSize{};
    }

    QIconEngine* IconEngine::clone() const {
        IconEngine* engine = new IconEngine(info_.lock());
        return engine;
    }

    QString IconEngine::key() const {
        return QStringLiteral("dingjing::IconEngine");
    }

    void IconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) {
        auto info = info_.lock();
        if(info) {
            info->internalQicon().paint(painter, rect, Qt::AlignCenter, mode, state);
        }
    }

    QPixmap IconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) {
        auto info = info_.lock();
        return info ? info->internalQicon().pixmap(size, mode, state) : QPixmap{};
    }

    void IconEngine::virtual_hook(int id, void* data) {
        auto info = info_.lock();
        switch(id) {
            case QIconEngine::AvailableSizesHook: {
                auto* args = reinterpret_cast<QIconEngine::AvailableSizesArgument*>(data);
                args->sizes = info ? info->internalQicon().availableSizes(args->mode, args->state) : QList<QSize>{};
                break;
            }
            case QIconEngine::IconNameHook: {
                QString* result = reinterpret_cast<QString*>(data);
                *result = info ? info->internalQicon().name() : QString{};
                break;
            }
            case QIconEngine::IsNullHook: {
                bool* result = reinterpret_cast<bool*>(data);
                *result = info ? info->internalQicon().isNull() : true;
                break;
            }
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
            case QIconEngine::ScaledPixmapHook: {
                auto* arg = reinterpret_cast<QIconEngine::ScaledPixmapArgument*>(data);
                arg->pixmap = info ? info->internalQicon().pixmap(arg->size, arg->mode, arg->state) : QPixmap{};
                break;
            }
#endif
        }
    }

}

#endif //LIBCORE_DINGJING_ICON_INFO_P_H
