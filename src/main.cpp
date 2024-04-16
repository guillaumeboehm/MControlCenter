/* Copyright (C) 2022  Dmitry Serov
 *
 * This file is part of MControlCenter.
 *
 * MControlCenter is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * MControlCenter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MControlCenter. If not, see <https://www.gnu.org/licenses/>.
 */

#include "cli.h"
#include "mainwindow.h"
#include "options.h"

#include <QApplication>
#include <QDBusConnectionInterface>
#include <QTranslator>

int main(int argc, char *argv[]) {
    const QString serviceName = "io.github.dmitry_s93.MControlCenter";

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(
            serviceName)) {

        // TODO: If we used commands -> communicate through dbus
        Options options;
        options.process_args(argc, argv);

        if (options.cli) {
            fprintf(stderr, "Executing CLI commands...\n");
            CLI cli;
            if (options.cooler_boost.has_value()) {
                cli.setCoolerBoost(options.cooler_boost.value());
            }

            if (socket->isOpen()) {
                socket->write("update");
                socket->flush();
                socket->close();
            }
            socket->deleteLater();
            return 0;
        } else {
            fprintf(stderr,
                    "Another instance of the application is already running\n");
        }
        return 0;
    }

    if (!QDBusConnection::sessionBus().registerService(serviceName)) {
        fprintf(stderr, "Failed to register D-Bus service\n");
        return 1;
    }

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "lang_" + QLocale(locale).name();
        if (translator.load(":/translations/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    // QObject::connect(&server, &QLocalServer::newConnection, [&w, &server]() {
    //     QLocalSocket *socket = server.nextPendingConnection();
    //     if (socket->waitForConnected() && socket->waitForReadyRead()) {
    //         QByteArray data = socket->readAll();
    //         if (std::strcmp(data.data(), "show") == 0) {
    //             w.show();
    //         } else if (std::strcmp(data.data(), "update") == 0) {
    //             w.externalUpdate();
    //         }
    //     }
    // });
    return QApplication::exec();
}
