import pytest

from openpilot.common.params import Params
from openpilot.system.updated.updated import Updater, BASEDIR, FINALIZED, OVERLAY_MERGED


@pytest.mark.parametrize(("device_type", "branch", "expected"), [
  ("tizi", "release3", "release-tizi"),
  ("tizi", "release3-staging", "release-tizi-staging"),
  ("mici", "release3", "release-mici"),
  ("mici", "release3-staging", "release-mici-staging"),
])
def test_target_branch_migration_from_current_branch(mocker, device_type, branch, expected):
  params = Params()
  params.remove("UpdaterTargetBranch")

  mocker.patch("openpilot.system.updated.updated.HARDWARE.get_device_type", return_value=device_type)
  mocker.patch.object(Updater, "get_branch", return_value=branch)

  assert Updater().target_branch == expected


@pytest.mark.parametrize(("device_type", "branch", "expected"), [
  ("tizi", "release3", "release-tizi"),
  ("tizi", "release3-staging", "release-tizi-staging"),
  ("mici", "release3", "release-mici"),
  ("mici", "release3-staging", "release-mici-staging"),
])
def test_target_branch_migration_from_param(mocker, device_type, branch, expected):
  params = Params()
  params.put("UpdaterTargetBranch", branch, block=True)

  mocker.patch("openpilot.system.updated.updated.HARDWARE.get_device_type", return_value=device_type)

  try:
    assert Updater().target_branch == expected
  finally:
    params.remove("UpdaterTargetBranch")


def test_restore_finalized_if_ready_restores_flag(mocker):
  updater = Updater()
  updater.branches["pre-build"] = "b" * 40
  mocker.patch.object(updater, "target_branch", "pre-build")
  mocker.patch.object(updater, "update_available", False)
  mocker.patch.object(updater, "get_commit_hash", side_effect=lambda path: {
    BASEDIR: "a" * 40,
    OVERLAY_MERGED: "b" * 40,
    FINALIZED: "b" * 40,
  }[path])
  mocker.patch.object(updater, "get_branch", return_value="pre-build")
  mocker.patch("openpilot.system.updated.updated.os.path.isdir", return_value=True)
  mock_flag = mocker.patch("openpilot.system.updated.updated.set_consistent_flag")

  assert updater.restore_finalized_if_ready() is True
  mock_flag.assert_called_once_with(True)


def test_restore_finalized_if_ready_skips_when_overlay_behind(mocker):
  updater = Updater()
  updater.branches["pre-build"] = "c" * 40
  mocker.patch.object(updater, "target_branch", "pre-build")
  mocker.patch.object(updater, "update_available", True)
  mocker.patch("openpilot.system.updated.updated.set_consistent_flag")

  assert updater.restore_finalized_if_ready() is False
