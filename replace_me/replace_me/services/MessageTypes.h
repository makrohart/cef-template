// Copyright (c) 2024 replace_me Authors. All rights reserved.
#pragma once

#include <string>
#include <vector>

#include "json.h"

namespace replace_me::json {

	struct CefQueryMessage
	{
		std::string action;
		std::string request;
		XPACK(O(action, request));
	};

	struct CefFileDialogRequest
	{
		std::string title;
		std::string defaultPath;
		int mode;
		XPACK(O(title, defaultPath, mode));
	};

	struct CefFileDialogResponse
	{
		std::string selectedPath;
		XPACK(O(selectedPath));
	};

	struct RepoPath
	{
		std::string path;
		XPACK(O(path));
	};

	struct CloneRepoRequest 
	{
		std::string url;
		std::string path;
		XPACK(O(url, path));
	};

	struct CheckoutBranchRequest
	{
		std::string repoPath;
		std::string branchName;
		XPACK(O(repoPath, branchName));
	};

	struct CreateBranchRequest
	{
		std::string repoPath;
		std::string branchName;
		std::string fromBranch;
		XPACK(O(repoPath, branchName, fromBranch));
	};

	struct DeleteBranchRequest
	{
		std::string repoPath;
		std::string branchName;
		XPACK(O(repoPath, branchName));
	};

	struct GetCommitsRequest
	{
		std::string repoPath;
		std::vector<std::string> branchNames;
		XPACK(O(repoPath, branchNames));
	};

	struct CreateCommitRequest
	{
		std::string repoPath;
		std::string message;
		std::string authorName;
		std::string authorEmail;
		XPACK(O(repoPath, message, authorName, authorEmail));
	};

	struct GetFileStatusRequest
	{
		std::string repoPath;
		XPACK(O(repoPath));
	};

	struct StageFilesRequest
	{
		std::string repoPath;
		std::vector<std::string> paths;
		XPACK(O(repoPath, paths));
	};

	struct UnstageFilesRequest
	{
		std::string repoPath;
		std::vector<std::string> paths;
		XPACK(O(repoPath, paths));
	};
	
	struct GetFileContentRequest
	{
		std::string repoPath;
		std::string filePath;
		std::string source;  // "workdir" or "index"
		XPACK(O(repoPath, filePath, source));
	};

	struct GetFileContentResponse
	{
		std::string content;
		std::string path;
		XPACK(O(content, path));
	};

	struct GetFileDiffRequest
	{
		std::string repoPath;
		std::string filePath;
		std::string source;  // "workdir" or "index"
		XPACK(O(repoPath, filePath, source));
	};

	struct GetFileDiffResponse
	{
		std::string diff;
		std::string path;
		XPACK(O(diff, path));
	};

	struct GetFileOriginalContentRequest
	{
		std::string repoPath;
		std::string filePath;
		XPACK(O(repoPath, filePath));
	};

	struct GetFileOriginalContentResponse
	{
		std::string content;
		std::string path;
		XPACK(O(content, path));
	};
	
	struct FileStatus {
	  std::string path;
	  // TODO: use enum instead of string
	  std::string status;  // "modified", "added", "deleted", "untracked", "renamed"
	  bool staged;
	  std::string diff;
	  XPACK(O(path, status, staged, diff));
	};

}  // namespace replace_me::services

	    