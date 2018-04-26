//
// PROJECT:         Aspia
// FILE:            ipc/ipc_channel.h
// LICENSE:         GNU General Public License 3
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#ifndef _ASPIA_IPC__IPC_CHANNEL_H
#define _ASPIA_IPC__IPC_CHANNEL_H

#include <QByteArray>
#include <QPointer>

#include <queue>
#include <utility>

class QLocalSocket;

namespace aspia {

class IpcServer;

class IpcChannel : public QObject
{
    Q_OBJECT

public:
    ~IpcChannel();

    static IpcChannel* createClient(QObject* parent = nullptr);

    void connectToServer(const QString& channel_name);

public slots:
    void disconnectFromServer();
    void readMessage();
    void writeMessage(int message_id, const QByteArray& buffer);

signals:
    void connected();
    void disconnected();
    void errorOccurred();
    void messageWritten(int message_id);
    void messageReceived(const QByteArray& buffer);

private slots:
    void onBytesWritten(qint64 bytes);
    void onReadyRead();

private:
    friend class IpcServer;

    IpcChannel(QLocalSocket* socket, QObject* parent);

    void scheduleWrite();

    using MessageSizeType = quint32;

    QPointer<QLocalSocket> socket_;

    std::queue<std::pair<int, QByteArray>> write_queue_;
    MessageSizeType write_size_ = 0;
    qint64 written_ = 0;

    bool read_required_ = false;
    bool read_size_received_ = false;
    QByteArray read_buffer_;
    MessageSizeType read_size_ = 0;
    qint64 read_ = 0;

    Q_DISABLE_COPY(IpcChannel)
};

} // namespace aspia

#endif // _ASPIA_IPC__IPC_CHANNEL_H