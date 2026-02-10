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
    params: Record<string, any> = {},
    callback?: (error: Error | null, result?: any) => void
  ): void {
    if (!window.cefQuery) {
      const error = new Error('CEF bridge is not available. cefQuery is not defined.');
      if (callback) {
        callback(error);
      } else {
        console.error(error);
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
            const result = JSON.parse(response);
            if (callback) {
              callback(null, result);
            }
          } catch (error) {
            const parseError = error instanceof Error 
              ? error 
              : new Error('Failed to parse response');
            if (callback) {
              callback(parseError);
            } else {
              console.error('Failed to parse CEF response:', parseError);
            }
          }
        },
        onFailure: (errorCode: number, errorMessage: string) => {
          const error = new Error(`CEF IPC error [${errorCode}]: ${errorMessage}`);
          if (callback) {
            callback(error);
          } else {
            console.error(error);
          }
        },
      });
    } catch (error) {
      const err = error instanceof Error ? error : new Error('Unknown error');
      if (callback) {
        callback(err);
      } else {
        console.error('Failed to invoke CEF IPC:', err);
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
      console.warn('CEF event bridge is not available. cefEventOn is not defined.');
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
      console.error(`Failed to register event listener for "${eventName}":`, error);
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
      console.warn('CEF event bridge is not available. cefEventEmit is not defined.');
      return;
    }

    try {
      window.cefEventEmit(eventName, data);
    } catch (error) {
      console.error(`Failed to emit event "${eventName}":`, error);
    }
  }
}

// 导出单例实例，方便直接使用
export const bridge = new Bridge();

// 导出便捷方法
export const invoke = bridge.invoke.bind(bridge);
export const on = bridge.on.bind(bridge);
export const emit = bridge.emit.bind(bridge);

/**
 * React Hook for using Bridge
 * @returns Bridge 实例
 */
export function useBridge(): Bridge {
  return useMemo(() => new Bridge(), []);
}
