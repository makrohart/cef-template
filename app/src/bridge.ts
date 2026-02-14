/**
 * CEF Bridge API
 * 提供与 CEF 后端通信的桥接层
 */

import { useMemo } from 'react';

// CEF 全局函数类型定义
interface CefQueryRequest {
  request: string;
  onSuccess?: (response: string) => void;
  onFailure?: (errorCode: number, errorMessage: string) => void;
  persistent?: boolean;
}

// 扩展 Window 接口以支持 CEF 函数
declare global {
  interface Window {
    /** CEF 查询函数，用于调用 IPC 方法 */
    cefQuery?: (request: CefQueryRequest) => number;
    /** 注册事件监听器 */
    cefEventOn?: (eventName: string, handler: (data: any) => void) => number;
    /** 移除事件监听器 */
    cefEventOff?: (eventName: string, listenerId: number) => void;
    /** 触发事件 */
    cefEventEmit?: (eventName: string, data: any) => void;
    /** 开发环境：便于在 DevTools Console 中调试，如 bridge.invoke / bridge.on / bridge.emit */
    bridge?: Bridge;
  }
}

/**
 * Bridge 类
 * 提供与 CEF 后端通信的接口
 */
export class Bridge {
  /**
   * 调用 CEF IPC 方法
   * @param ipcName IPC 方法名称
   * @param params 参数的 JSON 对象
   * @param callback 回调函数，接收响应或错误
   */
  invoke(
    ipcName: string,
    params: any,
    callback?: (errorCode: number | null, errorMessage: string) => void
  ): void {
    if (!window.cefQuery) {
      const errorMessage = "cefQuery is not defined.";
      const errorCode = -1;
      if (callback) {
        callback(errorCode, errorMessage);
      } else {
        console.error(`error [${errorCode}]: ${errorMessage}`);
      }
      return;
    }

    try {
      const request = JSON.stringify({
        action: ipcName,
        request: JSON.stringify(params),
      });

      window.cefQuery({
        request,
        onSuccess: (response: string) => {
          try {
            if (callback) {
              const result = JSON.parse(response);
              callback(null, result);
            }
          } catch (error) {
            const parseError = error instanceof Error 
              ? error 
              : new Error('Failed to parse response');
            if (callback) {
              callback(-1, parseError.message);
            } else {
              console.error(`error [${-1}]: ${parseError.message}`);
            }
          }
        },
        onFailure: (errorCode: number, errorMessage: string) => {
          if (callback) {
            callback(errorCode, errorMessage);
          } else {
            console.error(`error [${-1}]: ${errorMessage}`);
          }
        },
      });
    } catch (error) {
      const err = error instanceof Error ? error : new Error('Unknown error');
      if (callback) {
        callback(-1, err.message);
      } else {
        console.error(`error [${-1}]: ${err.message}`);
      }
    }
  }

  /**
   * 监听事件
   * @param eventName 事件名称
   * @param handler 事件处理函数
   * @returns 取消监听的函数
   */
  on(
    eventName: string,
    handler: (data: any) => void
  ): () => void {
    if (!window.cefEventOn) {
      const errorMessage = "cefEventOn is not defined.";
      const errorCode = -1;
      console.error(`error [${errorCode}]: ${errorMessage}`);
      return () => {};
    }

    try {
      // 调用 CEF 的 on 函数注册事件监听器
      const listenerId = window.cefEventOn(eventName, handler);
      
      // 返回取消监听的函数
      return () => {
        if (window.cefEventOff) {
          window.cefEventOff(eventName, listenerId);
        }
      };
    } catch (error) {
      const err = error instanceof Error ? error : new Error(`Failed to listen event for "${eventName}".`);
      console.error(`error [${-1}]: ${err.message}`);
      return () => {};
    }
  }

  /**
   * 触发事件
   * @param eventName 事件名称
   * @param data 事件数据
   */
  emit(eventName: string, data: any = {}): void {
    if (!window.cefEventEmit) {
      const errorMessage = "cefEventEmit is not defined.";
      const errorCode = -1;
      console.error(`error [${errorCode}]: ${errorMessage}`);
      return;
    }

    try {
      window.cefEventEmit(eventName, data);
    } catch (error) {
      const err = error instanceof Error ? error : new Error(`Failed to emit event for "${eventName}".`);
      console.error(`error [${-1}]: ${err.message}`);
    }
  }
}

// 导出单例实例，方便直接使用
export const bridge = new Bridge();

// 导出便捷方法
export const invoke = bridge.invoke.bind(bridge);
export const on = bridge.on.bind(bridge);
export const emit = bridge.emit.bind(bridge);

// 开发环境下挂到 window，方便在 DevTools Console 里用 bridge.invoke / bridge.on / bridge.emit
if (typeof window !== 'undefined' && import.meta.env?.DEV) {
  window.bridge = bridge;
}

/**
 * React Hook for using Bridge
 * @returns Bridge 实例
 */
export function useBridge(): Bridge {
  return useMemo(() => new Bridge(), []);
}
