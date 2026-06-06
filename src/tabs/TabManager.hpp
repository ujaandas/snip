#pragma once

#include <QAbstractListModel>

#include "TabData.hpp"

class TabManager : public QAbstractListModel {
  Q_OBJECT

 public:
  enum TabRoles { TitleRole = Qt::UserRole + 1, PathRole };

  explicit TabManager(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void openTab(const QString& title, const QString& path);
  Q_INVOKABLE void closeTab(int index);

 private:
  QList<TabData> tabs_;
};