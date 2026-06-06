#pragma once

#include <QAbstractListModel>

#include "TabData.hpp"

class TabManager : public QAbstractListModel {
  Q_OBJECT;

  Q_PROPERTY(
      int activeTab READ activeTab WRITE setActiveTab NOTIFY activeTabChanged);
  Q_PROPERTY(Editor* activeEditor READ activeEditor NOTIFY activeTabChanged);
  Q_PROPERTY(QString activeFilePath READ activeFilePath NOTIFY activeTabChanged);

 public:
  explicit TabManager(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  int activeTab() const;
  void setActiveTab(int index);
  Editor* activeEditor() const;
  QString activeFilePath() const;

  Q_INVOKABLE void openTab(const QString& title, const QString& path);
  Q_INVOKABLE void closeTab(int index);
  Q_INVOKABLE void closeActiveTab();
  Q_INVOKABLE void nextTab();
  Q_INVOKABLE void prevTab();

 signals:
  void activeTabChanged();

 private slots:
  void onEditorModifiedChanged();

 private:
  QList<TabData> tabs_;
  int activeTab_ = -1;
};